<script>
    export default {
        props: {
            value: ""
        }
    }
</script>

<template>
    <div id="card">
        <p id="value" >{{this.value}}</p>
    </div>
</template>

<style>
    #card {
        top: 340px;
        width: 200px;
        height: 300px;
        margin: 10px;
        border-radius: 10px;
        position: absolute;
        display: flex;
        align-items: center;
        justify-content: center;
        background-color: green;
    }
    #value {
        color: white;
        font-size: 100px;
    }
</style>