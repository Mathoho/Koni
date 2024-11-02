import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public  class TaskHello implements Runnable {
	public void run(String fileName) {
        String text = "Hello from a thread! "+Thread.currentThread().getName()
        +"\n";
        
        BufferedWriter out = null;
            try {
                File file = new File(fileName);
                out = new BufferedWriter(new FileWriter(file));
                out.write(text);
            } catch (IOException e) {
                e.printStackTrace();
            }

            //closing
            if (out != null) {
                try {
                    out.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
    }

    @Override
    public void run() {
        
    }		


}
