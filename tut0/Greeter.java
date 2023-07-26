public class Greeter {

    public static void greeter(){
        System.out.print("hello ");
    }

    public static void main(String[] args) {
        String name = args[0];
        greeter();
        System.out.println(name);
    }
}