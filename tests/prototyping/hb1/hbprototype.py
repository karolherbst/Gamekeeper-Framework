from GameKeeperModelPy import Game
from bs4 import BeautifulSoup

gameHTMLTree = [""]

def getEntryHTMLTag():
    return gameHTMLTree[0]

def setGameHTML(htmlString):
    soup = BeautifulSoup(htmlString, "html.parser")
    gameHTMLTree[0] = soup.find("div", {"id" : "regular_download_list"})

def parseGameHTML(storeGameId):
    gameListHTML = getEntryHTMLTag()
    gameHTML = gameListHTML.find("div", {"class" : storeGameId})
    gameInfoHTML = gameHTML.find("div", {"class" : "gameinfo"})
    gameTitleHTML = gameInfoHTML.find("div", {"class" : "title"})
    gameTitleLinkHTML = gameTitleHTML.find("a")
    gameSubTitleHTML = gameInfoHTML.find("div", {"class" : "subtitle"})
    gameSubTitleLinkHTML = gameSubTitleHTML.find("a")
    
    game = Game()
    game.id = storeGameId
    game.name = gameTitleLinkHTML.contents[0]
    return game

def parseGameIdsHTML(unused):
    result = []
    gameListHTML = getEntryHTMLTag()
    gameHTMLTags = gameListHTML.findAll("div", {"class" : "row"})
    for tag in gameHTMLTags:
        result.append(tag["class"][1]);
    return result
