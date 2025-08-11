package com.tuempresa.actorservice;

import akka.actor.typed.Behavior;
import akka.actor.typed.javadsl.Behaviors;

public class TaskActor {

    public static Behavior<Task.Command> create() {
        return Behaviors.receive(Task.Command.class)
            .onMessage(Task.Sum.class, TaskActor::onSum)
            .build();
    }

    private static Behavior<Task.Command> onSum(Task.Sum msg) {
        int sum = msg.a + msg.b;
        msg.replyTo.tell(new Task.Result(sum));
        return Behaviors.same();
    }
}
