import _ from "lodash";

export type RawData = {
  sensorlocation: string;
  humidity: string;
  temperature: string;
  timestamp: string;
};

export const formatData = (
  data: RawData[],
  dataLabelName: string,
  colorArray: string[]
) => {
  const groupedData = _(data).groupBy("sensorlocation").value();
  return Object.entries(groupedData).map(
    (
      [groupName, group]: [
        string,
        {
          sensorlocation: string;
          humidity: string;
          temperature: string;
          timestamp: string;
        }[]
      ],
      index
    ) => ({
      data: group
        .map((row) => ({
          x: parseInt(row.timestamp),
          y: parseFloat(row[dataLabelName]),
        }))
        .sort((row1, row2) => row1.x - row2.x),
      label: groupName,
      color:
        colorArray[Object.keys(groupedData).length > 4 ? index : index * 2],
    })
  );
};
