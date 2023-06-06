import { DynamoDBClient, ScanCommand } from "@aws-sdk/client-dynamodb";

const client = new DynamoDBClient({});

export const handler = async () => {
  const command = new ScanCommand({
    TableName: "home-iot-2023-data",
  });

  const response = await client.send(command);
  //console.log(response);

  // TODO add limits

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

  return JSON.stringify(data);
};
