/*
 * Helper functions for some examples
 */

import {Chess,SQUARES, type Square} from 'chess.js';
import type { Chessground } from 'svelte-chessground-ui';


// Find all legal moves
export function toDests(chess : Chess) {
	const dests = new Map();
	SQUARES.forEach(s => {
		const ms = chess.moves({square: s, verbose: true});
		if (ms.length) dests.set(s, ms.map(m => m.to));
	});
	return dests;
}
export function pawn_is_promoting(chess: Chess, orig: string, dest: string){
	let piece = chess.get(orig);
	if(piece.type == "p"){
		if(piece.color == "b"){
			if(orig[1] == "2" && dest[1] == "1"){
				return true;
			}
		}
		if(piece.color == "w"){
			if(orig[1] == "7" && dest[1] == "8"){
				return true;
			}
		}
	}
	return false;
}
// Play a move and toggle whose turn it is