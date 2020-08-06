cd /exports/projects/spark-3.0.0-preview2-bin-hadoop3.2
javac -cp jars/spark-core_2.12-3.0.0-preview2.jar:jars/scala-library-2.12.10.jar SparkSort.java
jar cvf SparkSort.jar SparkSort*.class

time -p bin/spark-submit --class SparkSort --master yarn --deploy-mode cluster --driver-memory 4g --executor-memory 2g --executor-cores 1 SparkSort.jar  /home/input/   /home/output/

