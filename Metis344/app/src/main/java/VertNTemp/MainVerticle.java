package VertNTemp;

import io.vertx.ext.web.Route;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;

import java.time.LocalDateTime;
import java.util.concurrent.CompletableFuture;


import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.http.HttpServer;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonObject;

public class MainVerticle extends AbstractVerticle{
    @Override
    public void start() throws Exception{
        
        //Router router = Router.router(vertx);
        
        StartVerxtMethods startVerxtMethods = new StartVerxtMethods();
        startVerxtMethods.purTimeEndPoint();
      //   router.route().handler(context -> {
      //       // Get the address of the request
      //       String address = context.request().connection().remoteAddress().toString();
      //       // Get the query parameter "name"
      //       MultiMap queryParams = context.queryParams();
      
      //       //TODO purchasing airtime
      //       String product = queryParams.contains("airtime") ? queryParams.get("airtime") : "data";
      //       String user = queryParams.contains("user") ? queryParams.get("user") : "unkown";
      //       String amount = queryParams.contains("amount") ? queryParams.get("amount") : "0";
      //       String cellNo = queryParams.contains("cellNo") ? queryParams.get("cellNo") : "0";
      //       int value = 30;
      //       //TODO localhost:8888/?product=airtime&user=koni&amount=40&cellNo=0735887432
      //       /*
      //        * String name = "John";
      //         int age = 30;
      //         String formattedString = String.format("My name is %s, and I am %d years old.", name, age);
      //        */
      //       // Write a json response
      //       //String msg = name, "purchased " + product +"of this amount " +" connected from " + address
      //       int amountPro = 0;
      //       try {
      //         amountPro = Integer.parseInt(amount);
      //       } catch (Exception e) {
      //         amountPro = 0;
      //         context.json(
      //           new JsonObject()
      //             .put("Error","Provide a valid Number, Can't convert the amount "+amount+" to Integer")
      //         );
      //         return;
      //       }

      //       String msg = String.format("%s purchased %s of amount R%d valued at %d on cellphone number %s and is connected from address %s .",
      //        user, product,amountPro,value,cellNo,address);
      //       context.json(
      //         new JsonObject()
      //           .put("product", product)
      //           .put("user", user)
      //           .put("amount", amount)
      //           .put("cellNo", cellNo)
      //           .put("message",msg)
      //       );
      //     });
      //   vertx.createHttpServer()
      // // Handle every request using the router
      // .requestHandler(router)
      // // Start listening
      // .listen(8888)
      // // Print the port
      // .onSuccess(server ->
      //   System.out.println(
      //     "HTTP server started on port " + server.actualPort()
      //   )
    //   // );
    //   router.post("/purchase/:user:product:amount:cellNo").handler(ctx -> {
    //     //this is the only end point the uaer needs 
    //         String product = ctx.pathParam("product");
    //         String user =  ctx.pathParam("user");
    //         int amount = 0;
    //         try {
    //           amount = Integer.parseInt(ctx.pathParam("amount"));
    //         } catch (Exception e) {
    //           String amntstr =  ctx.pathParam("user");
    //           String fstr = String.format("Error: Provide a valid Number, Can't convert the amount %s to Integer",amntstr);
    //           ctx.request().response().end(fstr);
    //           System.exit(0);
    //         }
    //         String cellNo =  ctx.pathParam("cellNo");
    //         //make Json
    //         ctx.request().response().end();
    //   });

    
    }
    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx();

        // Deploy the MainVerticle
        vertx.deployVerticle(new MainVerticle(), ar -> {
            if (ar.succeeded()) {
                System.out.println("MainVerticle deployed successfully");
            } else {
                System.err.println("MainVerticle deployment failed: " + ar.cause());
            }
        });
    
      //Hash
      //TODO http://localhost:8091/purchaseAirtime/?PAN=9874648236&amount=40&cellNo=0849871234
      //TODO http://localhost:8091/purchase/9874648236/30/0849871234
      //http://localhost:8091/hello/koni
      //http://localhost:8091/purchase/?Pan=9874648236&amount=30&cellNo=0849871234
      //http://localhost:8091/Module/CS343
    //     String db3name = "api3database";
    //     String owner = "konanani";
    //     String password = "123456";
    //     TemporalMethods tempMethods = new TemporalMethods();
    //     Vertx vertx = Vertx.vertx();
    //      //Connect to the API1, API2 and AP3 database
    //      API1Methods db1 = new API1Methods();
    //      db1.connect_to_db("api1databasek", "konanani", "123456");
 
    //      API2Methods db2 = new API2Methods();
    //      db2.connect_to_db("api2databasek", "konanani", "123456");
    //      System.out.println("starting server");
    //     HttpServer httpServer = vertx.createHttpServer();
    //   System.out.println("done with server");
    //     Router router = Router.router(vertx);
    //     System.out.println("vertx");
    //     // Route handler1 = router
    //     //         .get("/hello/:name")
    //     //         .handler(routingContext -> {
    //     //             String name = routingContext.request().getParam("name");
    //     //             System.out.println("came to hello: " + name);
    //     //             HttpServerResponse response = routingContext.response();
    //     //             response.setChunked(true);
            
    //     //             response.write("Hi " + name + "\n");
    //     //             response.end();
    //     //         });

    //     Route handler12 = router
    //     .post("/purchase")
    //     .consumes("*/json")
    //     .handler(routingContext -> {
    //         String pan = routingContext.request().getParam("Pan");
    //         String am = routingContext.request().getParam("amount");
    //         String cln = routingContext.request().getParam("cellNo");
    //         System.out.println("PRINTITNF came to purchase: " + pan);
    //         JsonObject jsonObject = new JsonObject()
    //             .put("Pan", pan)
    //             .put("amount", am)
    //             .put("cellNo", cln);

    //         // Convert the JSON object to a JSON string
    //         String jsonString = jsonObject.encode();
    //         HttpServerResponse response = routingContext.response();
    //         response.setChunked(true);
    //         response.write("hello");
    //         response.write(jsonString);
    //         response.end();
    //     });

    //     Route handler13 = router
    //     .get("/Module/:names")
    //     .handler(routingContext -> {
    //         String name = routingContext.request().getParam("names");
    //         System.out.println("came to Module: " + name);
    //         JsonObject jsonObject = new JsonObject()
    //             .put("names", name);
    //         // Convert the JSON object to a JSON string
    //         String jsonString = jsonObject.encode();
    //         HttpServerResponse response = routingContext.response();
    //         response.setChunked(true);
    //         response.write(jsonString);
    //         response.end();
    //     });

    //     Route handler14 = router
    //     .post("/ModuleNormal/<:names")
    //     .handler(routingContext -> {
    //         System.out.println("posint nomalr param");
    //         String name = routingContext.request().getParam("names");
    //         JsonObject jsonObject = new JsonObject()
    //             .put("names", name)
    //             .put("faith", "Jesus");
    //         // Convert the JSON object to a JSON string
    //         String jsonString = jsonObject.encode();
    //         HttpServerResponse response = routingContext.response();
    //         response.setChunked(true);
    
    //         response.write(jsonString);
    //         response.end();

    //     });

    //     Route handler2 = router
    //         .post("/purchaseAirtime/:PAN/:amount/:cellNo")
    //         .consumes("*/json")
    //         .handler(routingContext -> {
    //             HttpServerResponse response = routingContext.response();
    //             // String user = routingContext.request().getParam("user");
    //             //String product = routingContext.request().getParam("product");
    //             String phoneNumber = routingContext.request().getParam("cellNo");
    //             String PAN = routingContext.request().getParam("PAN");
    //             String amnt = routingContext.request().getParam("amount"); 
    //             final int amount = parseAmountParam(amnt);
    //             if (amount == -1) {
    //             String err = String.format("Error: Can not convert amount: %s to an integer",amnt);
    //             response.end(err);
    //             }
    //             String date = get_date();
    //             String time = get_time();
    //             CompletableFuture<String> paymentStatus = tempMethods.transactionTemp(PAN,amount,"PAYMENT",time,db1);
    //             paymentStatus.thenAccept(result -> {
    //                 if(result.equals("SUCCESS")){
    //                     System.out.println("payment was sucessful");
    //                     int userID = db1.get_user_id(PAN);
            
    //                     if (userID == -1) {
    //                         return;
    //                     }
            
    //                     final String dates = get_date();
    //                     final String times = get_time();
                        
    //                     // Add a transaction record in the API2 database
    //                     CompletableFuture<String> transSucOrFail = tempMethods.addTransTemp(userID,amount,dates,times,PAN,db2);
    //                     transSucOrFail.thenAccept(resultTranSuc -> {
    //                         if (resultTranSuc.equals("FAILED")) {
    //                             CompletableFuture<String> reversalStatus = tempMethods.transactionTemp(PAN, amount, "REVERSAL",time,db1);
    //                             reversalStatus.thenAccept(resultRev -> {
    //                                 if (resultRev.equals("SUCCESS")) {
    //                                     String err = "Failed transaction, funds returned";
    //                                     response.end(err);
    //                                 }
    //                             });
                                
    //                         } else if (resultTranSuc.equals("SUCCESS")) {
    //                             API3Methods db3 = new API3Methods();
    //                             //String pseudoName = "FalseName"; 
    //                             db3.connect_to_db(db3name, owner, password);
    //                             //handels asyc results
    //                             CompletableFuture<String> airtimePurchase = tempMethods.purAirtmTemp(phoneNumber, date, time, amount, PAN, db3);
    //                             airtimePurchase.thenAccept(resultAitm -> {
    //                                 if (resultAitm.equals("SUCCESS")) {
    //                                     tempMethods.updateTransTemp(date, time, "Completed", PAN, db2);
    //                                 }else if (resultAitm.equals("FAILED")) {
    //                                     tempMethods.updateTransTemp(date, time, "Failed", PAN, db2);
    //                                     CompletableFuture<String> reversalStatus = tempMethods.transactionTemp(PAN, amount, "REVERSAL",time,db1);
    //                                     reversalStatus.thenAccept(resultRevs ->{
    //                                         if (resultRevs.equals("SUCCESS")) {
    //                                             String err = "Failed transaction, funds returned";
    //                                             response.end(err);
    //                                         }
    //                                     });
    //                                 }else if (resultAitm.equals("ERROR")) {
    //                                     tempMethods.updateTransTemp(date, time, "Failed", PAN, db2);
    //                                     CompletableFuture<String> reversalStatus = tempMethods.transactionTemp(PAN, amount, "REVERSAL",time,db1);
    //                                     reversalStatus.thenAccept(resultRevs ->{
    //                                         if (resultRevs.equals("SUCCESS")) {
    //                                             String err = "Failed transaction, funds returned";
    //                                             response.end(err);
    //                                         }
    //                                     });
    //                                 }
    //                             });
    //                         }
    //                     });
    //                 }
    //             });
                
    //             System.out.println("came to post");
    //             response.setChunked(true);
    //             //response.write("processing transaction...");
    //             response.end("Done Transaction");
    //         //     routingContext
    //         //             .vertx()
    //         //             .setTimer(2000, tid -> routingContext.next());
    //             });
                
    //             // Route handler3 = router
    //             //     .post("/purchase/:user:product:amount:cellNo")
    //             //     .consumes("*/json")
    //             //     .handler(routingContext -> {
    //             //         System.out.println("came to post done processing");
    //             //         HttpServerResponse response = routingContext.response();
    //             //         response.write("HELLO"); //TODO display a sucess or failure to the user
    //             //         response.end("Done Transaction");
    //             //         // routingContext //TODO use this if you want add another and remove the response.end
    //             //         //         .vertx()
    //             //         //         .setTimer(2000, tid -> routingContext.next());
    //             // });

    //             // response.write("Hi TechPrimers from post\n");
    //             // response.end();

    //     // httpServer
    //     //     .requestHandler(router::accept)
    //     //     .listen(8091);
    //     httpServer.requestHandler(router).listen(8091);
    }
    public static String get_time() {

        LocalDateTime currentDateTime = LocalDateTime.now();
        int hour = currentDateTime.getHour();
        int minute = currentDateTime.getMinute();
        int second = currentDateTime.getSecond();
        return String.format("%02d:%02d:%02d", hour, minute, second);
    }

    public static String get_date() {
        LocalDateTime currentDateTime = LocalDateTime.now();
        int year = currentDateTime.getYear();
        int month = currentDateTime.getMonthValue();
        int day = currentDateTime.getDayOfMonth();
        return String.format("%04d-%02d-%02d", year, month, day);
    }

    public static int parseAmountParam(String amountParam) {
        int amount = 0;
        try {
            amount = Integer.parseInt(amountParam);
        } catch (NumberFormatException e) {
            // Handle the exception, e.g., log it or return a default value
            //System.err.println("Error: Cannot convert amount: " + amountParam + " to an integer");
            return -1;
        }
        return amount;
    }
    
}
//prchase route

