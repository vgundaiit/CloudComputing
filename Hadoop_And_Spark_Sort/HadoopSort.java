import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.Partitioner;
import org.apache.log4j.Logger;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class HadoopSort {

  private static final Logger logger = Logger.getLogger(HadoopSort.class);
  //Total number of ascii characters in generated gensort
  private static final int totalChars = 128;


  //Mapper that maps first 10 characters to key and rest to value in a line
  public static class TokenizerMapper
       extends Mapper<Object, Text, Text, Text>{

    public void map(Object key, Text value, Context context
                    ) throws IOException, InterruptedException {
      String line  = value.toString();
      String keypart = line.substring(0,10);
      String valuepart = line.substring(11,98);
      valuepart += "\r";
      context.write(new Text(keypart),new Text(valuepart));
    }
  }


  //Reducer that just writes to files and does nothing else.
  public static class IntSumReducer
       extends Reducer<Text,Text,Text,Text> {
    public void reduce(Text key, Iterable<Text> values,
                       Context context
                       ) throws IOException, InterruptedException {
      for (Text val : values) {
        context.write(key,val);
      }
    }
  }

  //This is the custom Partitioner that can be used to get sorted output among the partitions
  //This directs the output to reducers based on first character of the string.
  public static class customPartitioner extends Partitioner<Text,Text>{
                public int getPartition(Text key, Text value, int numReduceTasks){
      int numCharsPerReducer = totalChars/numReduceTasks;
      int firstChar = (int)key.toString().charAt(0);
      int iter = 0;
      while(iter < numReduceTasks){
        int start = iter * numCharsPerReducer;
        int end = (iter+1) * numCharsPerReducer;
        if(firstChar >= start && firstChar < end){
          return iter;
        }
        iter++;
      }
      return iter-1;
    }
  }

  public static void main(String[] args) throws Exception {
    logger.info("Starting timer");
    long start = System.currentTimeMillis();
    Configuration conf = new Configuration();
    Job job = Job.getInstance(conf, "hadoop sort");
    job.setJarByClass(HadoopSort.class);
    job.setMapperClass(TokenizerMapper.class);
    job.setCombinerClass(IntSumReducer.class);
    job.setPartitionerClass(customPartitioner.class);
    //Set the number of reducers here
    job.setReducerClass(IntSumReducer.class);
    job.setNumReduceTasks(1);
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(Text.class);
    FileInputFormat.addInputPath(job, new Path(args[0]));
    FileOutputFormat.setOutputPath(job, new Path(args[1]));

    if(job.waitForCompletion(true) == true){
      long finish = System.currentTimeMillis();
            long timeElapsed = finish - start;
            logger.info("Time elapsed in ms : \n");
            logger.info(timeElapsed);
            logger.warn("===============================================================================");
        System.exit(0);
    }
    else{
        System.exit(1);
    }
  }
}