<script lang="ts">
  import { onMount } from "svelte";
  import Chart from "./lib/Chart.svelte";
  import { formatData, type RawData } from "./lib/dataUtils";
  import Selector from "./lib/Selector.svelte";

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

  export let temperatureData = [];
  export let humidityData = [];
  export let selectedTimeRange = 4 * 24;
  let data: RawData[] = [];

  $: {
    temperatureData = [];
    fetch(
      import.meta.env.VITE_DATA_API_URL +
        (selectedTimeRange !== 0
          ? "?" + new URLSearchParams({ limit: selectedTimeRange.toString() })
          : ""),
      {
        mode: "cors",
        headers: {
          authorization: import.meta.env.VITE_API_KEY,
        },
      }
    )
      .then((response) => response.json())
      .then((responseData) => (data = responseData));
  }

  $: {
    temperatureData = formatData(data, "temperature", temperatureColors);
    humidityData = formatData(data, "humidity", humidityColors);
  }
</script>

<div>
  <p>🏠Home</p>
  <Selector bind:selectedValue={selectedTimeRange} />
  {#if temperatureData.length > 0}
    <Chart {temperatureData} {humidityData} />
  {:else}
    <p>Loading...</p>
  {/if}
</div>
