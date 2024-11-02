# Use Gradle image as builder
FROM gradle:jdk16 AS builder

# Set the working directory
WORKDIR /tut-project

# Copy Gradle files and source code
COPY build.gradle .
COPY settings.gradle .
COPY src src

# Build the Java application
RUN gradle clean build

# Runtime image
FROM openjdk:16

# Set the working directory
WORKDIR /tut-project

# Copy the JAR file from the builder stage
#COPY --from=build /tut-project/build/classes/java/main/org/example/my-java-Scheduler.jar .
COPY --from=builder /tut-project/build/libs/tut-project-1.0-SNAPSHOT.jar .



# Define the command to run the application
CMD ["java", "-jar", "tut-project-1.0-SNAPSHOT.jar"]

