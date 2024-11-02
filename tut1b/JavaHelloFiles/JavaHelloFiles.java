public class JavaHelloFiles{


    public static void main(String[] args) {
    int N = Integer.parseInt(args[0]);
    for (int i = 0; i < N; i++) {
			String threadName = i + " of " + N;
            String fileName = "p"+i+"lod.txt";
			TaskHello task = new TaskHello();
			Thread t = new Thread(() -> {task.run(fileName);}, threadName);
			t.start();
			
		}       
    }
}