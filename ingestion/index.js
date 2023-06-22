import { PutItemCommand, DynamoDBClient } from "@aws-sdk/client-dynamodb";

const client = new DynamoDBClient({});

export const handler = async (event) => {
  console.log(JSON.stringify(event));

  if (
    !Object.hasOwn(event.headers, "authorization") ||
    event.headers.authorization !== process.env.AUTH_KEY
  )
    throw new Error("Bad Authorization header");

  const data = JSON.parse(event.body);

  /*
   * data fields:
   *
   * temperature: number
   * humidity: number
   * sensor location: string
   */

  const fields = ["temperature", "humidity", "sensorlocation"];
  fields.map((field) => {
    if (!Object.hasOwn(data, field))
      throw new Error(`Data is missing field ${field}`);
  });

  const command = new PutItemCommand({
    TableName: "home-iot-2023-data",
    Item: {
      timestamp: { N: Date.now().toString() },
      temperature: { N: data.temperature.toString() },
      humidity: { N: data.humidity.toString() },
      sensorlocation: { S: data.sensorlocation.toString() },
    },
  });

  console.log(data);
  const response = await client.send(command);
  console.log(response);
  console.log("Saved data succesfully");
  return "";
};
