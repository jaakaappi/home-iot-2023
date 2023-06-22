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

  const id = (Math.random() + 1).toString(36).substring(7);
  let chart;
  let chartData = [];

  onMount(() => {
    chartData = temperatureData
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
        })) || []
      );
    const ctx = document.getElementById(id) as HTMLCanvasElement;
    chart = new Chart(ctx, {
      type: "line",
      data: {
        datasets: chart,
      },
      options: {
        elements: {
          point: {
            pointStyle: false,
          },
        },
        parsing: false,
        scales: {
          x: {
            type: "time",
            title: {
              display: true,
              text: "Time",
            },
          },
          y: {
            type: "linear",
            title: {
              display: true,
              text: "Temperature C°",
            },
          },
          y1: {
            type: "linear",
            display: true,
            position: "right",
            grid: {
              drawOnChartArea: false,
            },
            min: 0,
            title: {
              display: true,
              text: "Relative humidity %",
            },
          },
        },
      },
    });
  });

  $: if (chart) {
    chartData = temperatureData
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
        })) || []
      );
    chart.data.datasets = chartData;
    chart.update();
  }
</script>

<div style="position: relative; height:50vh; min-width:95vw">
  <canvas {id} style="margin: 0 auto; width: 100%; height: 100%;" />
</div>
