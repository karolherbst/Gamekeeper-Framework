from GameLibPy import Game
from bs4 import BeautifulSoup

def parseGameListHTML(htmlString):
    print("parsing HTML")
    soup = BeautifulSoup(htmlString, "html.parser")
    
    gameListHTML = soup.find("div", {"id" : "regular_download_list"})
    gameHTML = gameListHTML.find("div", {"class" : "airforte"})
    gameInfoHTML = gameHTML.find("div", {"class" : "gameinfo"})
    gameTitleHTML = gameInfoHTML.find("div", {"class" : "title"})
    gameTitleLinkHTML = gameTitleHTML.find("a")
    gameSubTitleHTML = gameInfoHTML.find("div", {"class" : "subtitle"})
    gameSubTitleLinkHTML = gameSubTitleHTML.find("a")
    
    game = Game()
    game.name(gameTitleLinkHTML.contents[0])
    game.website(gameTitleLinkHTML["href"])
    game.publisher(gameSubTitleLinkHTML.contents[0])
    print(game)
    return game
