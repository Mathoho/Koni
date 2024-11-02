//TODO: import the necessary libraries
import java.io.*;
import java.util.*;

public class JavaHelloRunnable {
	public static void main(String[] args) {
		//TODO: implement as described on SunLearn
		int N = Integer.parseInt(args[0]);
		for (int i = 0; i < N; i++){
			String threadName = i + " of " + N;
			TaskHello task = new TaskHello();
			Thread t = new Thread(() -> {task.run();}, threadName);
			t.start();
			
		}
	}
}

class TaskHello implements Runnable {
    //TODO: implement as described on SunLearn
	public void run() {
        System.out.println("Hello from a thread! " + Thread.currentThread().getName());
    }		

}
