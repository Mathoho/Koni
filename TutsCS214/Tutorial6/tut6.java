
public class tut6 {

    public static void main(String[] args) {
        int count = 0;
        int countnp = 0;
        int vertex = Integer.parseInt(args[0]);
        int edges = Integer.parseInt(args[1]);
        System.out.println("vertex = " + vertex);
        System.out.println("edges = " + edges);
        Graph G = new Graph(vertex);
        int i = 2;
        int r = 3;
        int stop = (args.length - 2) / 2;
        while (stop > 0) {
            int v = Integer.parseInt(args[i]);
            int w = Integer.parseInt(args[r]);
            System.out.println(v + " w = " + w);
            G.addEdge(v, w);
            stop = stop - 1;
            i = i + 2;
            r = r + 2;
        }
        int st = G.E();
        int ii =0;
        while (st > 0) {
            int s = Integer.parseInt(args[2 + ii]);
            DepthFirstPaths dfs = new DepthFirstPaths(G, s);
            int sti =G.E();
            int incre = 2;
            while (sti > 0) {
                if (dfs.hasPathTo(Integer.parseInt(args[incre]))){
                    count = count + 1;
                    incre = incre + 1;
               sti = sti-1;
                }else{
                    countnp = countnp +1;
                    incre = incre + 1;
                }
            }
            ii=ii+1;
            st = st-1;
        }
        int k = count;
        int m = count + countnp;
        double h = k/m;
       
    }
}