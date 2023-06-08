<script context="module" lang="ts">
  export type ChartData = {
    data: { x: number; y: number }[];
    label: string;
    color?: string;
    min?: number;
    max?: number;
  };
</script>

<script lang="ts">
  import { Chart, type ChartDataset } from "chart.js/auto";
  import { onMount } from "svelte";
  import "chartjs-adapter-luxon";

  export let temperatureData: ChartData[];
  export let humidityData: ChartData[] | undefined = undefined;
  export let irrigationData: ChartData[] | undefined = undefined;

  const id = (Math.random() + 1).toString(36).substring(7);

  onMount(() => {
    const ctx = document.getElementById(id) as HTMLCanvasElement;

    const chartData = temperatureData
      .map(
        (entry): ChartDataset => ({
          type: "line",
          label: entry.label,
          data: entry.data,
          cubicInterpolationMode: "monotone",
          borderColor: entry.color,
        })
      )
      .concat(
        humidityData?.map((entry) => ({
          type: "line",
          label: entry.label,
          data: entry.data,
          cubicInterpolationMode: "monotone",
          yAxisID: "y1",
          borderColor: entry.color,
        })) || [],
        irrigationData?.map((entry) => ({
          type: "bar",
          label: entry.label,
          data: entry.data,
          cubicInterpolationMode: "monotone",
          backgroundColor: entry.color,
          yAxisID: "y2",
        })) || []
      );

    console.log(chartData);

    new Chart(ctx, {
      type: "line",
      data: {
        datasets: chartData,
      },
      options: {
        parsing: false,
        scales: {
          x: {
            type: "time",
          },
          y: {
            type: "linear",
          },
          y1: {
            type: "linear",
            display: true,
            position: "right",
            grid: {
              drawOnChartArea: false,
            },
            min: 0,
            max: 100,
          },
          y2: {
            type: "linear",
            display: false,
            grid: {
              drawOnChartArea: false,
            },
          },
        },
      },
    });
  });
</script>

<canvas {id} style="height: 20vh; min-width: 50vw;" />
