import java.io.File;
import java.util.ArrayList;
import java.util.Scanner;

public class tut2 {

    Node root;
    Node left;
    Node right;
    int n = 0;

    public tut2() {

    }

    private class Node {

        Node uptrav;;
        Node travdownL;
        Node travdownR;
        int item;

        public Node(int val) {
            this.item = val;
         
            this.travdownL = null;
            this.travdownR = null;

        }

    }

    public Node insert(Node root, int[] arr, int k) {

        
        if (k < arr.length) {
            Node temp = new Node(arr[k]);
            root = temp;
          
            root.travdownL = insert(root.travdownL, arr, 2 * k + 1);
            root.travdownR = insert(root.travdownR, arr, (2 * k) + 2);

        }
        return root;
        
    }

    public int max() {
        int k = 0;
        return k;
    }

    private boolean less(int i, int j, int[] arr) {
    
           return (arr[i] - (arr[j])) < 0;
        
    }

    private void exch(int i, int j, int[] arr) {
        int t = arr[i];
        arr[i] = arr[j];
        arr[j] = t;
    }

    public void insert(int v, int[] arr) {
      
        if(n<arr.length){
    
        swim(n, arr);
      
        }
       n = n+1;
       
    }

    public int delMax() {
        int itm = root.item;
        return itm;
    }

    

    public boolean isEmpty() {
        if (n == 0) {
            return true;
        }

        return false;
    }

    public int size() {
        return n;
    }

    public void swim(int k, int[] arr) {
       
        while (k > 1 && less(k / 2, k, arr)) {
            exch(k / 2, k, arr);
            k = k / 2;
          
        }
       
    }

    private void sink(int k, int[] arr) {
        while (2 * k <= n) {
            int j = 2 * k;
            if (j < n && less(j, j + 1, arr))
                j++;
            if (!less(k, j, arr))
                break;
            exch(k, j, arr);
            k = j;
        }
    }

    public void inOrder(Node root) {
        if (root != null) {
            inOrder(root.travdownL);
            System.out.print(root.item + " ");
            inOrder(root.travdownR);
        }
    }

    public static void main(String[] args) {
        tut2 heap = new tut2();
        ArrayList<Integer> num = new ArrayList<Integer>();
        try {

            File file = new File(args[0]);
            Scanner sc = new Scanner(file);
            while (sc.hasNextLine()) {
                int no = sc.nextInt();
                num.add(no);

            }
        } catch (Exception e) {
            System.out.println("error ocurred");
        }
        int[] arr = new int[num.size()];
        for (int i = 0; i < arr.length; i++) {
            arr[i] = num.get(i);
           
            heap.insert(arr[i], arr);
          
        }
     heap.print(arr);
         heap.root = heap.insert(heap.root, arr, 0);
         System.out.println();
         heap.inOrder(heap.root);
       
    }

    public void print(int[] arr) {
        for (int i = 0; i < arr.length; i++) {
            System.out.print(arr[i] + "  ");
        }
    }
}
