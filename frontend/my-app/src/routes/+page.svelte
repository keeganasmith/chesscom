<script lang="ts">
    import {Chessground} from 'svelte-chessground-ui';
    import {Chess} from 'chess.js';
	import {onMount} from 'svelte';
	import {toDests,playOtherSide} from '$lib/util.js';
    import {retrieve_games} from "../common/requests"
    interface PGN {
        black_player: string;
        white_player: string;
        date: string;
        time: number;
        formatted_time: string;
    }
    interface Game {
        opponent: string;
        pgn: PGN;
        result: string;
        black_result: string;
        white_result: string;
        index: number;
    }
    const chess = new Chess();
	let chessground : Chessground;
    let username : string = "";
    let user_games: Array<Game> = [];
    let loaded_games: boolean = false;
    type Color = "white" | "black" | undefined;

    let turnColor: Color = "white"
	async function submit_username(){
        loaded_games = false;
        user_games = await retrieve_games(username);
        for(let i = 0; i < user_games.length; i++){
            let opponent = user_games[i]["pgn"]["black_player"];
            let result = user_games[i]["black_result"]
            if(username != user_games[i]["pgn"]["white_player"]){
                opponent = user_games[i]["pgn"]["white_player"];
                result = user_games[i]["white_result"]
            }
            user_games[i]["opponent"] = opponent;
            user_games[i]["result"] = result;
            user_games[i]["pgn"]["formatted_time"] = Math.floor(user_games[i]["pgn"]["time"] / 3600).toString() + ":" + Math.floor((user_games[i]["pgn"]["time"] % 3600) / 60).toString();
            user_games[i]["index"] = i;
        }
        loaded_games =true;
        console.log(user_games);
    }
    async function select_game(index: number){
        console.log(user_games[index]);
    }
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
    <label for="username">Enter your username:</label>
    <input type="text" id="username" bind:value={username}>
    <button on:click={submit_username}>Submit</button>
    {#if loaded_games}
    <table>
        <tr>
            <th>Opponent</th>
            <th>Date</th>
            <th>Time</th>
            <th>Result</th>
        </tr>
        {#each user_games as game}
            <tr on:click={select_game(game["index"])}>
                <td>{game["opponent"]}</td>
                <td>{game["pgn"]["date"]}</td>
                <td>{game["pgn"]["formatted_time"]}</td>
                <td>{game["result"]}</td>
            </tr>
        {/each}
    </table>
    {/if}
</div>