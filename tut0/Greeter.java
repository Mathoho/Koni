public class Greeter {

    public static void greeter(){
        System.out.print("hello ");
    }

    public static String getName(){
        Scanner scan = new Scanner(System.in);
        String nme = scan.nextLine();
        scan.close();
        return nme;
    } 
    public static void main(String[] args){
        String name = getName();
        greeter();
        System.out.println(name);
    }
}