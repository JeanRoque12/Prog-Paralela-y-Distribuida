package com.tuempresa.actorservice;

import akka.actor.typed.Behavior;
import akka.actor.typed.javadsl.AbstractBehavior;
import akka.actor.typed.javadsl.ActorContext;
import akka.actor.typed.javadsl.Receive;

public class Worker extends AbstractBehavior<Task.Command> {

    public static Behavior<Task.Command> create() {
        return akka.actor.typed.javadsl.Behaviors.setup(Worker::new);
    }

    private Worker(ActorContext<Task.Command> context) {
        super(context);
    }

    @Override
    public Receive<Task.Command> createReceive() {
        return newReceiveBuilder()
            .onMessage(Task.Sum.class, task -> {
                int suma = task.a + task.b;
                task.replyTo.tell(new Task.Result(suma));
                return this;
            })
            .build();
    }
}
