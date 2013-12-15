from GameLibPy import Game
from bs4 import BeautifulSoup

def getEntryHTMLTag(htmlString):
    print("parsing HTML")
    soup = BeautifulSoup(htmlString, "html.parser")
    return soup.find("div", {"id" : "regular_download_list"})

def parseGameHTML(htmlString, storeGameId):
    gameListHTML = getEntryHTMLTag(htmlString)
    gameHTML = gameListHTML.find("div", {"class" : storeGameId})
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

def parseGameIdsHTML(htmlString):
    result = []
    gameListHTML = getEntryHTMLTag(htmlString)
    gameHTMLTags = gameListHTML.findAll("div", {"class" : "row"})
    for tag in gameHTMLTags:
        result.append(tag["class"][1]);
    return result
