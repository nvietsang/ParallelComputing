import java.io.IOException;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

import com.sun.org.apache.bcel.internal.generic.NEW;


public class MeanTemperature {
	public static class MeanMapper extends Mapper<Object, Text, Text, Double> {
		
		@Override
		public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
			if (value == null) {
				return;
			}
			String[] values = value.toString().split((","));
			if (values.length != 3) {
				return;
			}
			String date = values[0];
			Text mmyyyy = new Text(date.substring(2));
			Double max_temp = Double.parseDouble(values[2]);
			System.out.println("Date: " + mmyyyy);
			context.write(mmyyyy, max_temp);
		}
	}
	
	public static class MeanReducer extends Reducer<Text, Double, Text, Double> {
		private Map meanMap = new HashMap<>();
		
		@Override
		public void reduce(Text key, Iterable<Double> values, Context context) throws IOException, InterruptedException {
			double sum = 0d;
			int count = 0;
			
			for (Double val: values) {
				if (val == null) {
					return;
				}
				sum += val;
				count++;
			}
			context.write(new Text(key), new Double(sum/count));
		}
		
	}
	
	public static void main(String[] args) throws IOException, ClassNotFoundException, InterruptedException {
		System.out.println("Hello");
		Configuration conf = new Configuration();
		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
		
		if (otherArgs.length != 2) {
			System.err.println("Usage: TopN <in> <out>");
			System.exit(2);
		}
		
		Job job = Job.getInstance(conf);
		job.setJobName("Mean Temperature");
		job.setJarByClass(MeanTemperature.class);
		job.setMapperClass(MeanMapper.class);
		
		job.setReducerClass(MeanReducer.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Double.class);
		FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
		FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}
