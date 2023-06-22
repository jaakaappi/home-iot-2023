<script lang="ts">
  const readingsPerHour = 4;
  const buttonValues = [
    { label: "24h", value: readingsPerHour * 24 },
    { label: "48h", value: readingsPerHour * 48 },
    { label: "7d", value: readingsPerHour * 24 * 7 },
    { label: "14d", value: readingsPerHour * 24 * 14 },
    { label: "30d", value: readingsPerHour * 24 * 30 },
    { label: "6m", value: readingsPerHour * 24 * 180 },
    { label: "1y", value: readingsPerHour * 24 * 365 },
    { label: "max", value: 0 },
  ];
  export let selectedValue = 0;
</script>

<div>
  {#each buttonValues as value, index}
    <button
      class={value.value === selectedValue ? "chosen" : ""}
      on:click={() => (selectedValue = value.value)}>{value.label}</button
    >
  {/each}
</div>

<style>
  button {
    margin: 4px;
  }
  button.chosen {
    background-color: grey;
  }
</style>
