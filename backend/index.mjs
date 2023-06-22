import { DynamoDBClient, ScanCommand } from "@aws-sdk/client-dynamodb";

const client = new DynamoDBClient({});

export const handler = async (event) => {
  if (
    !Object.hasOwn(event.headers, "authorization") ||
    event.headers.authorization !== process.env.AUTH_KEY
  )
    throw new Error("Bad Authorization header");

  let command = new ScanCommand({
    TableName: "home-iot-2023-data",
  });

  //console.log(event);

  if (
    event.hasOwnProperty("queryStringParameters") &&
    Object.keys(event.queryStringParameters).includes("limit")
  ) {
    console.log(event.queryStringParameters.limit);
    command.input.Limit = parseInt(event.queryStringParameters.limit);
  }

  //console.log(command);
  const response = await client.send(command);
  //console.log(response);

  const data = response.Items.flatMap((row) =>
    Object.entries(row)
      .map(([column, typeValuePair]) => {
        return {
          [column]: Object.values(typeValuePair)[0],
        };
      })
      .reduce((current, accumulated) => ({ ...current, ...accumulated }))
  );

  console.log(`Processed ${data.length} rows`);

  //console.log(data);

  return {
    statusCode: 200,
    headers: {
      //"Access-Control-Allow-Headers": "Content-Type,access-control-allow-headers,authorization",
      //"Access-Control-Allow-Origin": "*",
      //"Access-Control-Allow-Methods": "OPTIONS,POST,GET"
    },
    body: JSON.stringify(data),
  };
};
