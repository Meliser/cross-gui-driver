package org.kds;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.function.Consumer;
import java.util.concurrent.FutureTask;
import java.lang.reflect.InvocationTargetException;

import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.control.Control;
import javafx.scene.control.Tooltip;
// import javafx.scene.control.Button;
// import javafx.event.EventType;
// import javafx.event.Event;
// import javafx.event.EventHandler;
// import javafx.stage.Stage;
// import javafx.concurrent.Task;
import javafx.application.Platform;


// class CustomEvent extends Event {
//     public CustomEvent() {
//         super(PLASMA_STRIKE);
//     }
//     public static final EventType<CustomEvent> PLASMA_STRIKE = new EventType<>(Event.ANY, "PLASMA_STRIKE");
// }

// class StrikeEventHandler implements EventHandler<CustomEvent> {
//     @Override
//     public void handle(CustomEvent event) {
//         System.out.println("Reactor received strike:");

//         // event.consume();  if event is consumed the handler for the parent node will not be invoked.
//     }
// }



public class Controller {
    static public void walkTree() {
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

    static private void walkTreeImpl(Node node, Consumer<Node> visitor) {
        visitor.accept(node);
        if (node instanceof Parent) {
            ((Parent) node).getChildrenUnmodifiable()
                .forEach(n -> walkTreeImpl(n, visitor));
        }
    }

    static public String getText(String name) throws NoSuchMethodException,
                                                    SecurityException,
                                                    IllegalAccessException,
                                                    IllegalArgumentException,
                                                    InvocationTargetException,
                                                    InterruptedException,
                                                    TimeoutException,
                                                    Throwable {
        var task = new FutureTask<String>(() -> {
            System.out.println("getText GUI thread " + Thread.currentThread().getId());
            var list = javafx.stage.Window.getWindows();
            var listIter = list.iterator();
            while(listIter.hasNext()) {
                var w = listIter.next();
                var scene = w.getScene();
                var node = scene.lookup("#"+name);
                if (node == null) {
                    continue;
                }
    
                var nodeClass = node.getClass();
                var m = nodeClass.getMethod("getText", (Class<?>[])null);
                var res = m.invoke(node, (Object[])null);
                return (String)res;

            }
            throw new RuntimeException("Name is not found");
        });
        Platform.runLater(task);
        try {
            return task.get(5, TimeUnit.SECONDS);
        }
        catch(ExecutionException e) {
            System.out.println("OOOps");
            throw e.getCause();
        }
    }
}
