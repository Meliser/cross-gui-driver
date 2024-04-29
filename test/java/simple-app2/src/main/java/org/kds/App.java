package org.kds;

import java.util.function.Consumer;

import javafx.application.Application;
import javafx.stage.Stage;
import javafx.scene.Scene;
import javafx.scene.Node;
import javafx.scene.Group;
import javafx.scene.Parent;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Control;
import javafx.scene.control.Tooltip;
import javafx.scene.layout.FlowPane;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;

import org.kds.Plugin;


/**
 * JavaFX App
 */
public class App extends Application {

    @Override
    public void start(Stage stage) {
        System.out.println("GUI main thread " + Thread.currentThread().getId());
         
        Button btn = new Button();
        btn.setText("Click!");
        btn.setId("btn1");
        btn.setTooltip(new Tooltip(btn.getId()));
         
        btn.setOnAction(new EventHandler<ActionEvent>() {
             
            @Override
            public void handle(ActionEvent event) {
                //walkTree();
            }
        });
         
        Label label = new Label("Hello");
        label.setId("Label1");
        label.setTooltip(new Tooltip(label.getId()));
        
        Group group = new Group(btn);

        FlowPane root = new FlowPane(label, group);
        Scene scene = new Scene(root, 300, 150);
        stage.setScene(scene);
         
        stage.show();
    }

    public static void main(String[] args) {
        System.out.println("Main thread " + Thread.currentThread().getId());
        Plugin nativeLib = new Plugin();
        nativeLib.init_driver("NICE");
        launch();
    }

}