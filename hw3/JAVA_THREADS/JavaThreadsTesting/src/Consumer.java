import java.util.Random;

public class Consumer extends Thread{
    public Consumer(String name){
        super(name);
    }
    private void consume() throws InterruptedException {
        SharedData.lock.lock();
        while(SharedData.buffer.isEmpty()) {
            try {
                System.err.println("Consumer Thread: " + Thread.currentThread().getName() + " is waiting");
                SharedData.ready.await();
            } catch (InterruptedException e) {
                SharedData.lock.unlock();
                throw new RuntimeException(e);
            }
        }
        SharedData.Product p = SharedData.buffer.remove(0);
        System.out.println("Consumer Thread: " + Thread.currentThread().getName() +" completed consumption for: "+p.name+"\nGoing to rest");
        sleep(500);
        SharedData.ready.signalAll();
        SharedData.lock.unlock();

    }

    public void run(){
        Random rand = new Random();
        while(true){
            try {
                consume();
                sleep(rand.nextInt(SharedData.rand_lower_bound,SharedData.rand_upper_bound));
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }
    }
}
