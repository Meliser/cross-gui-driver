package kds.org;

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
         
        Button btn = new Button();
        btn.setText("Click!");
         
        btn.setOnAction(new EventHandler<ActionEvent>() {
             
            @Override
            public void handle(ActionEvent event) {
                //walkTree();
            }
        });
         
        Label label = new Label("Hello");               
        Group group = new Group(btn);
        label.setTooltip(new Tooltip("SOME TOOL"));
         
        FlowPane root = new FlowPane(label, group);     
        Scene scene = new Scene(root, 300, 150);    
        stage.setScene(scene);
         
        stage.show();
    }

    public static void main(String[] args) {
        Plugin nativeLib = new Plugin();
        nativeLib.init_driver("NICE");
        launch();
    }

}