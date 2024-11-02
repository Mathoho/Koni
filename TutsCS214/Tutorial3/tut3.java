
public class tut3{
    
    public static void draw(BST<Integer, Integer> st , int max) {
        StdDraw.setCanvasSize(600, 800);
        StdDraw.setXscale(0, st.size());
        StdDraw.setYscale(0,max+1);
        double a = 0;
        for (int s : st.levelOrder()){
            StdDraw.filledRectangle(0.5 + a, 0, 0.05, s/2);
      a=a+0.5;
        }
      
    }
    public static void main(String[] args) {
        BST<Integer, Integer> st = new BST<Integer, Integer>();
      
            In in = new In();
            int[] arr =  in.readAllInts();
            
   int max = 0 ;
   for(int i = 0 ; i < arr.length ; i++){
       if(max<arr[i]){
           max=arr[i];
       }
   }
        for(int i = 0 ; i < arr.length;i++){
            st.put( arr[i], arr[i]);
        }
        draw(st,max);

    }

}