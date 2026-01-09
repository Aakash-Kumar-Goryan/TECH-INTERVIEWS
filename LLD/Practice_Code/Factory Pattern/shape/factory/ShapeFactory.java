package shape.factory;

import shape.Shape;
import shape.Circle;
import shape.Rectangle;

public class ShapeFactory {

    public Shape getShape(String input) {
        return switch (input) {
            case "CIRCLE" -> new Circle();
            case "Rectangle" -> new Rectangle();
            default -> null;
        };
    }
}