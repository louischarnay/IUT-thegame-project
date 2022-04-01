<script>
import Card from "./Card.vue";
    export default {
    methods: {
        startDrag (evt, item) {
            evt.dataTransfer.dropEffect = 'move'
            evt.dataTransfer.effectAllowed = 'move'
            evt.dataTransfer.setData('itemvalue', item.value)
        },
    },
    components: { Card }
}        
</script>

<template>
    <div>
        <div class="stacks">
            <div id="stack"></div>
            <div id="stack"></div>
            <div id="stack"></div>
            <div id="stack"></div>
        </div>
        <div class="drag-el" draggable @dragstart="startDrag($event, item)">
            <Card value="1"/>
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