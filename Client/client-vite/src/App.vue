<script setup>
import Card from "./components/Card.vue";

</script>

<template>
    <div>
        <div class="stacks">
            <div id="stack"></div>
            <div id="stack"></div>
            <div id="stack"></div>
            <div id="stack"></div>
        </div>
        <div class="cards">
            <Card value="0"/>
        </div> 
    </div>
</template>

<style>
    .stacks{
        position: absolute;
        display: flex;
        align-items: center;
        text-align: center;
    }
    #stack {
        width: 200px;
        height: 300px;
        border-radius: 10px;
        background-color: red;
        margin: 10px;
    }
</style>