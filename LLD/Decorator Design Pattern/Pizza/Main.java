
import basepizza.BasePizza;
import basepizza.Margherita;
import basepizza.toppings.ExtraCheese;
import basepizza.toppings.Mushroom;



public class Main {
    public static void main(String args[]) {
        // Margherita + ExtraCheese
        BasePizza pizza1 = new ExtraCheese(new Margherita());
        System.out.println(pizza1.cost());

        // Margherita + ExtraCheese + Mushroom
        BasePizza pizza2 = new Mushroom(new ExtraCheese(new Margherita()));
        System.out.println(pizza2.cost());
    }
}