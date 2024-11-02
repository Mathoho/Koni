package VertNTemp;

import io.vertx.core.MultiMap;
import io.vertx.core.Vertx;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;

public class StartVerxtMethods {
    
    public StartVerxtMethods(){

    }
    public void purTimeEndPoint(){
//http://localhost:8888/?name=koni
//http://localhost:8888/purchase/?name=koni&product=airtime&amount=30&PAN=1001&cellNo=0759871234
//http://localhost:8888/getDataValues
        Vertx vertx = Vertx.vertx();
        Router router = Router.router(vertx);
 //TODO items[user, product,amount, pan, amountProduct, cellNo]
    // Mount the handler for all incoming requests at every path and HTTP method
    router.get("/purchase").handler(context -> {
      System.out.println("cane to purchase");
      // Get the address of the request
      String address = context.request().connection().remoteAddress().toString();
      // Get the query parameter "name"
      MultiMap queryParams = context.queryParams();
      String name = queryParams.contains("name") ? queryParams.get("name") : "unknown"; //nmae of user
      String product = queryParams.contains("product") ? queryParams.get("product") : "unknown"; //tproduct type artime/data
      String amount = queryParams.contains("amount") ? queryParams.get("amount") : "unknown";  //
      String value = queryParams.contains("value") ? queryParams.get("value") : "unknown";
      String PAN = queryParams.contains("PAN") ? queryParams.get("PAN") : "unknown";
      String cellNo = queryParams.contains("cellNo") ? queryParams.get("cellNo") : "unknown";
      // Write a json response
      LogicApi api = new LogicApi();
      String[] items = {name,product,amount,PAN,value,cellNo};
      String msg = api.Logic(items);
      context.json(
        new JsonObject()
          .put("name", name)
          .put("product", product)
          .put("amount", amount)
          .put("amountProduct", value)
          .put("message", msg)
      );
    });

    router.get("/getDataValues").handler(context -> {
      // Define the logic for this endpoint
      System.out.println("came to getting the data valuse");
      context.json(
        new JsonObject()
          .put("400MB", "R30")
          .put("600MB", "R60")
          .put("500MB", "R20")
          .put("900MB", "R100")
          .put("2GB", "R130")
      );
    });

    router.post("/purchasepp").handler(context -> {
      JsonObject json = context.body().asJsonObject();
      if (json != null) {
          String name = json.getString("name", "unknown");
          String product = json.getString("product", "unknown");
          String amount = json.getString("amount", "unknown");
          String value = json.getString("value", "unknown");
          String PAN = json.getString("PAN", "unknown");
          String cellNo = json.getString("cellNo", "unknown");
          // Rest of your logic
          LogicApi api = new LogicApi();
          String[] items = {name,product,amount,PAN,value,cellNo};
          String msg = api.Logic(items);
          System.out.println("came to purchase");
          //Formulating the responce
          JsonObject responseJson = new JsonObject()
            .put("name", name)
            .put("product", product)
            .put("amount", amount)
            .put("amountProduct", value)
            .put("message", msg);

        // Set the HTTP status code to 200 (OK)
        context.response()
            .setStatusCode(200)
            .end(responseJson.encode());

      } else {
          context.response().setStatusCode(400).end("Invalid JSON data in the request body.");
      }
  });
    // Create the HTTP server
    vertx.createHttpServer()
      // Handle every request using the router
      .requestHandler(router)
      // Start listening
      .listen(8888)
      // Print the port
      .onSuccess(server ->
        System.out.println(
          "HTTP server started on port " + server.actualPort()
        )
      );
    }
}
