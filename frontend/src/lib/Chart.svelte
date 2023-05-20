<script lang="ts">
  import "chartist/dist/index.css";
  import { LineChart, AutoScaleAxis, Interpolation } from "chartist";
  import { onMount } from "svelte";

  const generateDateTimes = (hours: number): number[] =>
    Array.from(Array(hours).keys()).map(
      (_, index) => new Date().getTime() - index * 60 * 60 * 1000
    );

  const generateTemperatureData = (timestamps: number[]) =>
    timestamps.map((timestamp) => ({ x: timestamp, y: Math.random() * 100 }));

  onMount(() => {
    const data = generateTemperatureData(generateDateTimes(5));

    console.log(data);

    new LineChart(
      "#chart",
      {
        series: [data],
      },
      {
        axisX: {
          type: AutoScaleAxis,
          labelInterpolationFnc: (value, index) => {
            if (index % 2 === 0) {
              return new Date(value).toLocaleString(undefined, {
                hour: "2-digit",
                minute: "2-digit",
              });
            } else {
              return null;
            }
          },
          labelOffset: { x: -20, y: 0 },
        },
        axisY: {
          type: AutoScaleAxis,
          divisor: 5,
        },
        fullWidth: true,
        lineSmooth: Interpolation.simple({
          fillHoles: true,
        }),
        chartPadding: {
          right: 20,
        },
        low: 0,
      }
    );
  });
</script>

<div id="chart" style="height: 50vh; width: 50vw;" />
