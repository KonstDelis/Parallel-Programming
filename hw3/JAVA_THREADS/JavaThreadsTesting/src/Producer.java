import java.util.Random;

public class Producer extends Thread{
    public Producer(String name){
        super(name);
    }
    private void produce() throws InterruptedException {
        SharedData.lock.lock();
        while(SharedData.buffer.size()>SharedData.storage_capacity){
            try {
                System.err.println("Producer Thread: " + Thread.currentThread().getName() +" is waiting");
                SharedData.ready.await();
            } catch (InterruptedException e) {
                SharedData.lock.unlock();
                throw new RuntimeException(e);
            }
        }
        SharedData.Product p = new SharedData.Product();
        SharedData.buffer.add(p);
        System.out.println("Producer Thread: " + Thread.currentThread().getName() +" completed production for: "+p.name +"\nGoing to rest");
        sleep(500);
        SharedData.ready.signalAll();
        SharedData.lock.unlock();
    }

    public void run(){
        Random rand = new Random();
        while(true){
            try {
                produce();
                sleep(rand.nextInt(SharedData.rand_lower_bound,SharedData.rand_upper_bound));
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }
    }
}
