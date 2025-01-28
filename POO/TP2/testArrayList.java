import java.util.ArrayList;



public class testArrayList {
        public static void main(String[] args) {
            ArrayList<String> charList = new ArrayList<>();
            charList.add("brrr");
            charList.add(0,"en");
            for(String e : charList) System.out.print(e + " ");
            
        }
}
