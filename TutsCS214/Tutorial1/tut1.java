
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class tut1 {

   public static void name(Comparable[] arr, String k) {
      String sort = k.substring(0, 6); 
      System.out.println(k);
      Stopwatch fst = new Stopwatch();
      if (!k.equals(sort)) {
         StdRandom.shuffle(arr);
      }
      Insertion.sort(arr);
      System.out.println("InsertionSort :    " + fst.elapsedTime());

      Stopwatch fst1 = new Stopwatch();
      if (!k.equals(sort) && !k.equals("Duplic")) {
         StdRandom.shuffle(arr);
      }
      Merge.sort(arr);
      System.out.println("MergeSort     :    " + fst1.elapsedTime());

      Stopwatch fst2 = new Stopwatch();
      if (!k.equals(sort) && !k.equals("Duplic")) {
         StdRandom.shuffle(arr);
      }
      Quick.sort(arr);
      System.out.println("QuickSort     :    " + fst2.elapsedTime());

      Stopwatch fst3 = new Stopwatch();
      if (!k.equals(sort) && !k.equals("Duplic")) {
         StdRandom.shuffle(arr);
      }
      Shell.sort(arr);
      System.out.println("ShellSort     :    " + fst3.elapsedTime());

      Stopwatch fst4 = new Stopwatch();
      if (!k.equals(sort) && !k.equals("Duplic")) {
         StdRandom.shuffle(arr);
      }
      Selection.sort(arr);
      System.out.println("SelectionSort :    " + fst4.elapsedTime());

   }

public static Comparable[] fil(String srt) {
   Comparable[] arr = new Comparable[1000];

   int i = 0;
   try {
      File myObj = new File(srt);
      Scanner myReader = new Scanner(myObj);

      while (i < 1000) {
         arr[i] = myReader.nextLine();
         i = i + 1;
      }
   } catch (FileNotFoundException e) {
      System.out.println("An error occurred.");
      e.printStackTrace();
   }
   return arr;
}

   public static void main(String[] args) {
      // for(int i = 0 ; i < 1000000; i ++) {
      // int rand = (int) (Math.random()*10000000);
      // System.out.println(rand);
      // }

      // for(int i = 0 ; i < 1000000; i ++) {
      // double rand = (Math.random()*10000000);
      // System.out.println(rand);
      // }

      // int rand =0;
      // String chr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
      // for(int i = 0 ; i < 1000000;i++)
      // {
      // rand = (int) (Math.random()*20) +1;
      // String word = "";

      // for(int j = 0 ; j< rand ; j++)
      // {

      // int rndchr = (int) (Math.random()*20) + 1;
      // word = word + chr.charAt(rndchr);
      // }
      // System.out.println(word);
      // }
      // int[] a= {3,56,54,32,12,1,8,9,6,44,22};
      // System.out.println("unsorted array");
      // print(a);
      // int r = a.length -1;
      // sort(a,0,r);
      // System.out.println();
      // print(a);
      // }

     
      // int randint = 0;
      // for (int i = 0; i < 500000; i++) {
      //    randint = (int) (Math.random() * 1000000);
      //    System.out.println(randint);
      //    System.out.println(randint);
      // }
     
      // String chr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
      // int randstr = 0;
      // for(int i = 0 ; i < 500000;i++)
      // {
      // randstr = (int) (Math.random()*20) +1;
      // String word = "";

      // for(int j = 0 ; j< randstr ; j++)
      // {

      // int rndchr = (int) (Math.random()*20) + 1;
      // word = word + chr.charAt(rndchr);
      // }
      // System.out.println(word);
      // System.out.println(word);
      // }
      
      //  for(int i = 0 ; i < 500000; i ++) {
      // double rand = (Math.random()*1000000);
      // System.out.println(rand);
      // System.out.println(rand);
      // }

      //start of new code

      //INTEGERS
     Comparable[] arr = fil("randomInts.txt");
      name(arr, "Random Integers");
      System.out.println();
      name(arr, "Sorted Integers:");
      System.out.println();
      arr = fil("dupInts.txt");
      name(arr, "Duplicate Integers:");  

      //DOUBLES
      arr = fil("randomDoubles.txt");
      name(arr, "Random Double");
      System.out.println();
      arr = fil("dupDoubles.txt");
      name(arr, "Duplicate Double");
      System.out.println();
      name(arr, "Sorted Double");
      System.out.println();

      //STRINGS
      arr = fil("randomStrings.txt");
      name(arr, "Random Strings");
      System.out.println();
      arr = fil("dupStrings.txt");
      name(arr, "Duplicate Strings");
      System.out.println();
      name(arr, "Sorted Strings");
      System.out.println();   
   }

}
