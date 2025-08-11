package com.tuempresa.actorservice;

import com.amazonaws.services.lambda.runtime.Context;
import com.amazonaws.services.lambda.runtime.RequestHandler;
import com.fasterxml.jackson.databind.ObjectMapper;

import akka.actor.typed.ActorSystem;
import akka.actor.typed.javadsl.AskPattern;
import akka.util.Timeout;

import java.time.Duration;
import java.util.concurrent.CompletionStage;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

public class LambdaHandler implements RequestHandler<Object, String> {

    private static final ObjectMapper mapper = new ObjectMapper();

    // ActorSystem con tipo de mensaje Task.Command (según diseño típico Akka Typed)
    private static final ActorSystem<Task.Command> actorSystem = ActorSystem.create(Supervisor.create(), "ActorSystem");

    @Override   
    public String handleRequest(Object rawInput, Context context) {
        try {
            // DEBUG: Log the raw input to see what we're actually receiving
            System.out.println("DEBUG - Raw input type: " + rawInput.getClass().getName());
            System.out.println("DEBUG - Raw input content: " + mapper.writeValueAsString(rawInput));
            
            // Try to extract the actual input
            Input inputObj;
            if (rawInput instanceof java.util.Map) {
                // API Gateway event format
                @SuppressWarnings("unchecked")
                java.util.Map<String, Object> event = (java.util.Map<String, Object>) rawInput;
                
                if (event.containsKey("body")) {
                    // API Gateway proxy integration
                    String body = (String) event.get("body");
                    System.out.println("DEBUG - Event body: " + body);
                    inputObj = mapper.readValue(body, Input.class);
                } else {
                    // Direct invocation or non-proxy integration
                    inputObj = mapper.convertValue(event, Input.class);
                }
            } else {
                // Direct Input object (shouldn't happen based on your logs)
                inputObj = (Input) rawInput;
            }
            
            // DEBUG: Log what we received after parsing
            System.out.println("DEBUG - Parsed input: a=" + inputObj.a + ", b=" + inputObj.b);
            
            // Timeout de 5 segundos para la comunicación con el actor
            Duration timeout = Duration.ofSeconds(5);

            // Preguntar al actor usando AskPattern
            CompletionStage<Task.Result> resultFuture = AskPattern.ask(
                actorSystem,
                replyTo -> {
                    System.out.println("DEBUG - Sending to actor: a=" + inputObj.a + ", b=" + inputObj.b);
                    return new Task.Sum(inputObj.a, inputObj.b, replyTo);
                },
                timeout,
                actorSystem.scheduler()
            );

            // Wait for result with explicit timeout
            Task.Result result = resultFuture.toCompletableFuture().get(4, TimeUnit.SECONDS);
            
            System.out.println("DEBUG - Actor returned: " + result.value);

            // Crear objeto de salida y convertirlo a JSON
            Output outputObj = new Output(result.value);
            String jsonResponse = mapper.writeValueAsString(outputObj);
            System.out.println("DEBUG - Final JSON response: " + jsonResponse);
            
            return jsonResponse;

        } catch (TimeoutException e) {
            System.out.println("ERROR - Request timed out");
            return "{\"error\": \"Request timed out\"}";
        } catch (Exception e) {
            System.out.println("ERROR - Exception: " + e.getMessage());
            e.printStackTrace();
            return "{\"error\": \"" + e.getMessage() + "\"}";
        }
    }

    // Clase interna para parsear entrada JSON
    public static class Input {
        public int a;
        public int b;
        
        // Default constructor (required for Jackson)
        public Input() {
            System.out.println("DEBUG - Input constructor called");
        }
        
        // Setters (required for Jackson to populate fields)
        public void setA(int a) { 
            System.out.println("DEBUG - setA called with: " + a);
            this.a = a; 
        }
        
        public void setB(int b) { 
            System.out.println("DEBUG - setB called with: " + b);
            this.b = b; 
        }
        
        // Getters (sometimes Jackson needs these too)
        public int getA() { return a; }
        public int getB() { return b; }
    }

    // Clase interna para salida JSON
    public static class Output {
        public int result;
        public Output(int result) {
            this.result = result;
        }
        
        // Getter for Jackson serialization
        public int getResult() { return result; }
    }
}