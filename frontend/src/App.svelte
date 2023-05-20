<script lang="ts">
  import Chart, { type ChartData } from "./lib/Chart.svelte";

  const temperatureData: ChartData[] = [
    {
      data: [
        { x: 1000000, y: Math.ceil(Math.random() * 100 - 50) },
        { x: 2000000, y: Math.ceil(Math.random() * 100 - 50) },
        { x: 3000000, y: Math.ceil(Math.random() * 100 - 50) },
        { x: 4000000, y: Math.ceil(Math.random() * 100 - 50) },
      ],
      label: "Data",
      color: "#df5700", // #df5700 #e87000 #ee8002 #f48f05 #f89b08 #f8aa29 #f9b94f #fbcd81 #fce1b3 #fef3e0
    },
    {
      data: [
        { x: 1000000, y: Math.ceil(Math.random() * 100 - 50) },
        { x: 2000000, y: Math.ceil(Math.random() * 100 - 50) },
        { x: 3000000, y: Math.ceil(Math.random() * 100 - 50) },
        { x: 4000000, y: Math.ceil(Math.random() * 100 - 50) },
      ],
      label: "Data",
      color: "#ee8002", // #df5700 #e87000 #ee8002 #f48f05 #f89b08 #f8aa29 #f9b94f #fbcd81 #fce1b3 #fef3e0
    },
    {
      data: [
        { x: 1000000, y: Math.ceil(Math.random() * 100 - 50) },
        { x: 2000000, y: Math.ceil(Math.random() * 100 - 50) },
        { x: 3000000, y: Math.ceil(Math.random() * 100 - 50) },
        { x: 4000000, y: Math.ceil(Math.random() * 100 - 50) },
      ],
      label: "Data",
      color: "#f89b08", // #df5700 #e87000 #ee8002 #f48f05 #f89b08 #f8aa29 #f9b94f #fbcd81 #fce1b3 #fef3e0
    },
  ];
  const humidityData: ChartData[] = [
    {
      data: [
        { x: 1000000, y: Math.ceil(Math.random() * 100) },
        { x: 2000000, y: Math.ceil(Math.random() * 100) },
        { x: 3000000, y: Math.ceil(Math.random() * 100) },
        { x: 4000000, y: Math.ceil(Math.random() * 100) },
      ],
      label: "Data 1",
      color: "#1b3cbc", // #1b3cbc #115ddb #0270ee #0082ff #0091ff #1fa2ff #54b3ff #8ac8ff #b9ddff #e2f2ff
    },
    {
      data: [
        { x: 1000000, y: Math.ceil(Math.random() * 100) },
        { x: 2000000, y: Math.ceil(Math.random() * 100) },
        { x: 3000000, y: Math.ceil(Math.random() * 100) },
        { x: 4000000, y: Math.ceil(Math.random() * 100) },
      ],
      label: "Data 2",
      color: "#0270ee",
    },
    {
      data: [
        { x: 1000000, y: Math.ceil(Math.random() * 100) },
        { x: 2000000, y: Math.ceil(Math.random() * 100) },
        { x: 3000000, y: Math.ceil(Math.random() * 100) },
        { x: 4000000, y: Math.ceil(Math.random() * 100) },
      ],
      label: "Data 3",
      color: "#0082ff",
    },
  ];
  const irrigationData: ChartData[] = [
    {
      data: [
        { x: 1000000, y: Math.ceil(Math.random() * 100) },
        { x: 2000000, y: Math.ceil(Math.random() * 100) },
        { x: 3000000, y: Math.ceil(Math.random() * 100) },
        { x: 4000000, y: Math.ceil(Math.random() * 100) },
      ],
      label: "Data 1",
      color: "#008b00", // #008b00 #09af00 #41c300 #61d800 #75e900 #90ee02 #aaf255 #c6f68d #defabb #f2fde4
    },
    {
      data: [
        { x: 1000000, y: Math.ceil(Math.random() * 100) },
        { x: 2000000, y: Math.ceil(Math.random() * 100) },
        { x: 3000000, y: Math.ceil(Math.random() * 100) },
        { x: 4000000, y: Math.ceil(Math.random() * 100) },
      ],
      label: "Data 2",
      color: "#41c300",
    },
    {
      data: [
        { x: 1000000, y: Math.ceil(Math.random() * 100) },
        { x: 2000000, y: Math.ceil(Math.random() * 100) },
        { x: 3000000, y: Math.ceil(Math.random() * 100) },
        { x: 4000000, y: Math.ceil(Math.random() * 100) },
      ],
      label: "Data 3",
      color: "#75e900",
    },
  ];
</script>

<div>
  <div>
    <p>🏠Indoors</p>
    <Chart {temperatureData} {humidityData} />
  </div>
  <div>
    <p>☀️Outdoors</p>
    <Chart {temperatureData} {humidityData} {irrigationData} />
  </div>
</div>
