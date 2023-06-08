<script lang="ts">
  import { onMount } from "svelte";
  import Chart from "./lib/Chart.svelte";
  import { formatData } from "./lib/dataUtils";

  const temperatureColors = [
    "#df5700",
    "#e87000",
    "#ee8002",
    "#f48f05",
    "#f89b08",
    "#f8aa29",
    "#f9b94f",
    "#fbcd81",
    "#fce1b3",
    "#fef3e0",
  ];

  const humidityColors = [
    "#1b3cbc",
    "#115ddb",
    "#0270ee",
    "#0082ff",
    "#0091ff",
    "#1fa2ff",
    "#54b3ff",
    "#8ac8ff",
    "#b9ddff",
    "#e2f2ff",
  ];
  // #1b3cbc #115ddb #0270ee #0082ff #0091ff #1fa2ff #54b3ff #8ac8ff #b9ddff #e2f2ff
  // #008b00 #09af00 #41c300 #61d800 #75e900 #90ee02 #aaf255 #c6f68d #defabb #f2fde4

  export let temperatureData = [];
  export let humidityData = [];

  onMount(async function () {
    console.log(import.meta.env.VITE_API_KEY);

    const response = await fetch(import.meta.env.VITE_DATA_API_URL, {
      mode: "cors",
      headers: {
        authorization: import.meta.env.VITE_API_KEY,
      },
    });
    const data = await response.json();
    console.log(data);

    temperatureData = formatData(data, "temperature", temperatureColors);
    humidityData = formatData(data, "humidity", humidityColors);
  });
</script>

<div>
  <div>
    <p>🏠Indoors</p>
    {#if temperatureData.length > 0}
      <Chart {temperatureData} {humidityData} />
    {/if}
  </div>
  <div>
    <p>☀️Outdoors</p>
    <Chart temperatureData={[]} humidityData={[]} irrigationData={[]} />
  </div>
</div>
