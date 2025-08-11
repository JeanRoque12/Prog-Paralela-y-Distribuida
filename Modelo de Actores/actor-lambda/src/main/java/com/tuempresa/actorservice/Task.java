package com.tuempresa.actorservice;

import akka.actor.typed.ActorRef;

public interface Task {
    interface Command {}

    class Sum implements Command {
        public final int a;
        public final int b;
        public final ActorRef<Result> replyTo;

        public Sum(int a, int b, ActorRef<Result> replyTo) {
            this.a = a;
            this.b = b;
            this.replyTo = replyTo;
        }
    }

    class Result {
        public final int value;
        public Result(int value) {
            this.value = value;
        }
    }
}
