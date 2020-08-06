import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.PairFunction;
import scala.Tuple2;

public class SparkSort {

    public static void main(String[] args) {
       
		SparkConf sc = new SparkConf().setAppName("Spark Sort");
		JavaSparkContext sparkContext = new JavaSparkContext(sc);
		//get command line arguments for input and output path
		String input = args[0];
        String output = args[1];
		
		//Iteratively convert every line in the text file to an RDD
        JavaRDD<String> textFile = sparkContext.textFile(input);
		
		//A function that returns a key-value RDD pairs
        PairFunction<String, String, String> keyValuePairs =
               new PairFunction<String, String, String>() {
                    public Tuple2<String, String > call(String x) throws Exception{
                        return new Tuple2(x.substring(0,10), x.substring(11,98)); //Specifying values for key-value by using beginning and end indexs
                    }
                };
				
								
		//Generate pair RDD according to KeyValuePairs Tuple and run sort
        JavaPairRDD<String, String> pairs = textFile.mapToPair(keyValuePairs).sortByKey(true);
        pairs.map(x -> x._1 + " " + x._2 + "\r").coalesce(1).saveAsTextFile(output); //Format the output to remove default brackets and
																					 //append "\r" at the end of each line.
																					 //Merge all data partitioned and save as a file on disk
       
    }
}
