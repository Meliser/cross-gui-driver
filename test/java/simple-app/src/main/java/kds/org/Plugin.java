package kds.org;

import java.util.function.Consumer;

import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.control.Control;
import javafx.scene.control.Tooltip;

public class Plugin {
    public void walkTree() {
        var list = javafx.stage.Window.getWindows();
        var listIter = list.iterator();
        while(listIter.hasNext()){
            var w = listIter.next();
            var root = w.getScene().getRoot();
            //System.out.println(root);
            walkTreeImpl(root,node -> {
                if (node instanceof Control) {
                    ((Control) node).setTooltip(new Tooltip(node.getClass().toString()));
                    System.out.println(node.getClass());
                }
            });
        }
    }

    private void walkTreeImpl(Node node, Consumer<Node> visitor) {
        visitor.accept(node);
        if (node instanceof Parent) {
            ((Parent) node).getChildrenUnmodifiable()
                .forEach(n -> walkTreeImpl(n, visitor));
        }
    }
}
