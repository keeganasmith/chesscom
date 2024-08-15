<script lang="ts">
    import {Chessground} from 'svelte-chessground-ui';
    import {Chess} from 'chess.js';
	import {onMount} from 'svelte';
	import {toDests,playOtherSide} from '$lib/util.js';
    const chess = new Chess();
	let chessground : Chessground;
    type Color = "white" | "black" | undefined;

    let turnColor: Color = "white"
	
	onMount(async () => {
		chessground.set( {
			movable: { 
                free: false, 
                dests: toDests(chess), 
                events: {
                    after: playOtherSide( chessground, chess ) 
                } 
            }
		});
	});
    
</script>
<div>
    <Chessground bind:this={chessground} {turnColor}/>
</div>