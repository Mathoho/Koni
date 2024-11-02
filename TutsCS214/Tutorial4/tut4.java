import java.io.File;
import java.io.FileNotFoundException;
import java.util.LinkedList;
import java.util.Scanner;


public class tut4{
   private int M;
   private LinkedList<String>[] st;

   public tut4(int tsz){
    this.M = tsz;
    @SuppressWarnings("unchecked")
    LinkedList<String>[] st = (LinkedList<String>[]) new LinkedList[M];
    this.st = st;
    for(int i = 0 ; i < M ; i++){
        st[i] = new LinkedList<String>();
    }
   }

   private int hash(String key) { 
       return (key.hashCode() & 0x7fffffff) % M; 
    }

    public void put(String key) {
         st[hash(key)].add(key);
         }

    public int index_number(String s){
        int k = 0;
        for(int i = 0 ; i < s.length() ; i++){
            try {
                String a = ""+s.charAt(i);
                k = Integer.parseInt(a);
                return i;
             } catch (Exception e) {
                 
            }
        }
       
        return k;
    }

    public boolean inDic(String key){
        int val = hash(key);
        LinkedList<String> inx = st[val];
        for(int i = 0 ; i < inx.size(); i++){
            if(inx.contains(key)){
                return true;
            }
        }
        return false;
    }
    
    
    public static void main(String[] args) {
    tut4 t4 = new tut4(997);
    String password = args[0];

    //if length is less than 12
    if (password.length()<12) {
      System.out.println("Bad password");  
      System.exit(0);
    } 
    try{
    //if length is greater 11 
    File f = new File("Dictionary");
      try (Scanner sc = new Scanner(f)) {
        while(sc.hasNextLine()){
              String str = sc.nextLine();
                t4.put(str);
          }
    } 
    } catch (FileNotFoundException e) {
        e.printStackTrace();
    }
    // if word in dictionary
    if(t4.inDic(password)){
        System.out.println("Bad password");  
        System.exit(0);
    }
   // word followed by a digit
int end = t4.index_number(password);
String sub = password.substring(0, end);
int tru = password.length()-end;
if(end!=0 && t4.inDic(sub) && tru==1 ){
    System.out.println("Bad password");  
    System.exit(0);
}

//if they are words in dictionary and separated by an int
int endtwo = -1;
if(end!=0 && tru !=1){
    String subpass = password.substring(end+1);
    endtwo = t4.index_number(subpass);

if(t4.inDic(sub) && endtwo == -1){
    System.out.println("Bad password");  
    System.exit(0);
}
}
System.out.println("Good password");  

    }
}