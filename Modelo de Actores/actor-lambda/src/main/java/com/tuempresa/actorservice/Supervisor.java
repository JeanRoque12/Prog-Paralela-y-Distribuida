package com.tuempresa.actorservice;

import akka.actor.typed.Behavior;
import akka.actor.typed.SupervisorStrategy;
import akka.actor.typed.javadsl.*;
import akka.actor.typed.ActorRef;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Supervisor extends AbstractBehavior<Task.Command> {

    private final List<ActorRef<Task.Command>> workers = new ArrayList<>();
    private final Random random = new Random();

    public static Behavior<Task.Command> create() {
        return Behaviors.supervise(
            Behaviors.setup(Supervisor::new))
            .onFailure(SupervisorStrategy.restart());
    }

    private Supervisor(ActorContext<Task.Command> context) {
        super(context);
        for (int i = 0; i < 5; i++) {
            ActorRef<Task.Command> worker = context.spawn(Worker.create(), "worker-" + i);
            workers.add(worker);
        }
    }

    @Override
    public Receive<Task.Command> createReceive() {
        return newReceiveBuilder()
            .onMessage(Task.Command.class, task -> {
                int idx = random.nextInt(workers.size());
                workers.get(idx).tell(task);
                return this;
            })
            .build();
    }
}
