public class Greeter {

    public static void greeter(){
        System.out.print("hello ");
    }

    public static void greeter(String name){
        System.out.println("hello "+name+" how are you today");
    }

    public static void main(String[] args) {
        String name = args[0];
        greeter(name);
    }
}