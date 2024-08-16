<script lang="ts">
    import {Chessground} from 'svelte-chessground-ui';
    import {Chess} from 'chess.js';
	import {onMount} from 'svelte';
	import {toDests,pawn_is_promoting} from '$lib/util.js';
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
    let promotion_display: boolean = false;
    let promotion_piece: string = "";
    let orig_square: string = "";
    let promote_square: string = "";
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
    function promote_piece(choice: string){
        chess.move({from: orig_square, to: promote_square, promotion: choice});
        chessground.set({fen: chess.fen()});
        const color = chess.turn() == 'w' ? 'white' : 'black';
        chessground.set({
            turnColor: color,
            movable: {
                color: color,
                dests: toDests(chess)
            }
        });
        promotion_display = false;
    }
    function playOtherSide(chessground : Chessground,chess : Chess) {
        return (orig : string,dest : string) => {
            if(promotion_display){
                chessground.set({ fen: chess.fen()});
                return;
            }
            if(pawn_is_promoting(chess, orig, dest)){
                promotion_display = true;
                orig_square = orig;
                promote_square = dest;
                return;
            }
            let chess_move = chess.move({ from: orig, to: dest });
            if (chess_move.flags === 'e') {
                chessground.set({ fen: chess.fen() })
            }
            const color = chess.turn() == 'w' ? 'white' : 'black';
            chessground.set({
                    turnColor: color,
                    movable: {
                        color: color,
                        dests: toDests(chess)
                    }
                });
            };
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
    <div id="chessboard" class="chessboard">
        <Chessground bind:this={chessground} {turnColor}/>
        {#if promotion_display}
        <div id="promotion-overlay" class="overlay">
            <button id="promote-queen" on:click={() => promote_piece('q')}>Queen</button>
            <button id="promote-rook" on:click={() => promote_piece('r')}>Rook</button>
            <button id="promote-bishop" on:click={() => promote_piece('b')}>Bishop</button>
            <button id="promote-knight" on:click={() => promote_piece('k')}>Knight</button>
        </div>
        {/if}
    </div>
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
            <tr on:click={() => select_game(game["index"])}>
                <td>{game["opponent"]}</td>
                <td>{game["pgn"]["date"]}</td>
                <td>{game["pgn"]["formatted_time"]}</td>
                <td>{game["result"]}</td>
            </tr>
        {/each}
    </table>
    {/if}
</div>
<style>
    .overlay {
        position: absolute;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
        background-color: rgba(0, 0, 0, 0.8); /* Semi-transparent background */
        display: flex;
        justify-content: center;
        align-items: center;
        z-index: 10; /* Ensure it's on top of other elements */
    }

    .overlay button {
        margin: 10px;
        padding: 10px 20px;
        font-size: 16px;
        color: white;
        background-color: #444;
        border: none;
        cursor: pointer;
    }
    .chessboard {
        position: relative
    }
</style>