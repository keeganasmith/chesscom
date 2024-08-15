const dev_url:string = "http://127.0.0.1:18080";

async function retrieve_games(username: string){
    let endpoint: string = dev_url + "/retrieve_games/" + username + "/10";
    let result = await (await fetch(endpoint)).json();
    return result;
}
export { retrieve_games }