<script lang="ts">
    import {Chessground} from 'svelte-chessground-ui';
    import {Chess} from 'chess.js';
	import {onMount} from 'svelte';
	import {toDests,playOtherSide} from '$lib/util.js';
    const chess = new Chess();
	let chessground : Chessground;
    let turnColor = "white"
    
	
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
<h1>Welcome to SvelteKit</h1>
<p>Visit <a href="https://kit.svelte.dev">kit.svelte.dev</a> to read the documentation</p>
<Chessground bind:this={chessground} {turnColor}/>