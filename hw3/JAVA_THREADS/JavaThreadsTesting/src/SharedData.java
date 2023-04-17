import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Scanner;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class SharedData {
    static ReentrantLock lock = new ReentrantLock();
    public static Condition ready = lock.newCondition();

    public static int rand_upper_bound, rand_lower_bound, storage_capacity;
    static class Product{
        public final String name;
        private static int number=0;
        public Product(){
            name = "product"+number++;
        }
    }
    public static LinkedList<Product> buffer = new LinkedList<Product>();

    public static void main(String[] args){
        Scanner cin = new Scanner(System.in);

        System.out.println("Enter number of producers: ");
        int producer_num = cin.nextInt();
        if(producer_num<=0 || producer_num>10){
            System.err.println("Error: producer num must be between 1 and 10");
            System.exit(1);
        }
        System.out.println("Enter number of consumers: ");
        int consumer_num = cin.nextInt();
        if(consumer_num<=0 || consumer_num>10){
            System.err.println("Error: consumer num must be between 1 and 10");
            System.exit(1);
        }
        System.out.println("Enter storage capacity (buffer size for products): ");
        storage_capacity = cin.nextInt();
        if(storage_capacity<=0){
            System.err.println("Error: storage capacity must be >0");
            System.exit(1);
        }
        System.out.println("Enter lower bound for rand in ms (used when producers/consumers are resting): ");
        rand_lower_bound = cin.nextInt();
        if(rand_lower_bound<=0){
            System.err.println("Error: consumer num must be >0");
            System.exit(1);
        }
        System.out.println("Enter upper bound for rand in ms (used when producers/consumers are resting): ");
        rand_upper_bound = cin.nextInt();
        if(rand_upper_bound<=rand_lower_bound){
            System.err.println("Error: upper bound must be >rand_lower_bound");
            System.exit(1);
        }


        Producer[] producers = new Producer[producer_num];
        Consumer[] consumers = new Consumer[consumer_num];

        for(int i=0; i<producer_num; i++){
            producers[i] = new Producer("p"+i);
        }

        for(int i=0; i<consumer_num; i++){
            consumers[i] = new Consumer("c"+i);
        }

        for(int i=0; i<Integer.max(consumer_num,producer_num); i++){
            if(i<consumer_num) consumers[i].start();
            if(i<producer_num) producers[i].start();
        }

        try {
            for(int i=0; i<Integer.max(consumer_num,producer_num); i++){
                if(i<consumer_num) consumers[i].join();
                if(i<producer_num) producers[i].join();
            }
        }
        catch (InterruptedException e) {
            throw new RuntimeException(e);
        }

    }

}
