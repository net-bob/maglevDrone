import java.util.*;

public class pokerSim {

    public static class Card {
        private int val;
        private String suit;

        public Card(int val, String suit){
            this.val = val;
            this.suit = suit;
        }
        
        public int getVal(){
            return val;
        }
        public String getSuit(){
            return suit;
        }

        public String toString(){
            if (val == 1){
                return "A" + suit;
            }
            else if (val == 11){
                return "J" + suit;
            }
            else if (val == 12){
                return "Q" + suit;
            }
            else if (val == 13){
                return "K" + suit;
            }
            return val + suit;
        }
    }
    
    public static class Deck {
        private int size = 52;
        private ArrayList<Card> deck = new ArrayList<Card>();

        public Deck(boolean generate){
            if (generate){
                for (int num = 1; num <= size / 4; num++){
                    for (int suit = 0; suit < 4; suit++){
                        if (suit == 0){
                            deck.add(new Card(num, "S"));
                        }
                        else if (suit == 1){
                            deck.add(new Card(num, "C"));
                        }
                        else if (suit == 2){
                            deck.add(new Card(num, "H"));
                        }
                        else if (suit == 3){
                            deck.add(new Card(num, "D"));
                        }
                    }
                }
            }
        }

        public void shuffleDeck(){
            Collections.shuffle(deck);
        }
        public void printDeck(){
            for (int i = 0; i < size; i++){
                System.out.println(deck.get(i));
            }
        }
        public void addCardToDeck(Card card){
            deck.add(card);
        }

        public ArrayList<Card> getDeck(){
            return deck;
        }
        public Card drawCard(){
            return deck.remove(0);
        }
    }

    public static class PlayerHand {
        private int money;
        private String name;
        private Card card1;
        private Card card2;

        public PlayerHand(Card card1, Card card2){
            this.card1 = card1;
            this.card2 = card2;
            money = 1000;
        }

        public PlayerHand(){
            card1 = null;
            card2 = null;
            money = 1000;
        }
        
        public PlayerHand(Deck deck, String name, int money){
            card1 = deck.drawCard();
            card2 = deck.drawCard();
            this.name = name;
            this.money = money;
        }

        public PlayerHand(Deck deck, String name) {
            card1 = deck.drawCard();
            card2 = deck.drawCard();
            this.name = name;
            this.money = 1000;
        }

        public Card getCard1(){
            return card1;
        }
        public Card getCard2(){
            return card2;
        }

        public String toString(){
            return name + ": " + card1 + " " + card2 + " $" + money;
        }

    }

    public static class CommunityCards {
        ArrayList<Card> communityCards = new ArrayList<Card>();

        public void addCards(Deck deck, int numOfCards){
            for (int i = 0; i < numOfCards; i++){
                communityCards.add(deck.drawCard());
            }
        }
        public void addCard(Card card){
            communityCards.add(card);
        }

        public ArrayList<Card> getCommunityCards(){
            return communityCards;
        }
    }

    public static PlayerHand compareHands(ArrayList<PlayerHand> playerHands, CommunityCards communityCards){

        ArrayList<PlayerHand> bestPlayerHands = new ArrayList<PlayerHand>();
        int bestHand = 11;
        
        for (int playerHand = 0; playerHand < playerHands.size(); playerHand++){
            int currentHandStrongest = detectBestPokerHand(playerHands.get(playerHand), communityCards);
            System.out.println(currentHandStrongest);
            if (currentHandStrongest < bestHand){
                bestHand = currentHandStrongest;
                bestPlayerHands = new ArrayList<PlayerHand>();
                bestPlayerHands.add(playerHands.get(playerHand));
            }
            else if (currentHandStrongest == bestHand) {
                bestPlayerHands.add(playerHands.get(playerHand));
            }
        }

        int max = -1;
        PlayerHand bestPlayer = null;
        if (bestPlayerHands.size() == 1){
            return bestPlayerHands.get(0);
        }
        if (bestHand == 2){
            for (PlayerHand playerHand : playerHands){
                ArrayList<Card> totalCards = new ArrayList<Card>();
                totalCards.add(playerHand.getCard1());
                totalCards.add(playerHand.getCard2());
                for (int i = 0; i < communityCards.getCommunityCards().size(); i++){
                    totalCards.add(communityCards.getCommunityCards().get(i));
                }
                sortCardsByVal(totalCards);

                int check = checkStraightFlush(totalCards);
                if (check > max) {
                    max = check;
                    bestPlayer = playerHand;
                }
                System.out.println(max);
            }
            return bestPlayer;
        }
        if (bestHand == 3){
            for (PlayerHand playerHand : playerHands){
                ArrayList<Card> totalCards = new ArrayList<Card>();
                totalCards.add(playerHand.getCard1());
                totalCards.add(playerHand.getCard2());
                for (int i = 0; i < communityCards.getCommunityCards().size(); i++){
                    totalCards.add(communityCards.getCommunityCards().get(i));
                }
                sortCardsByVal(totalCards);

                int check = checkQuads(totalCards);
                if (check == 1){
                    return playerHand;
                }
                else if (check > max) {
                    max = check;
                    bestPlayer = playerHand;
                }
                System.out.println(max);
            }
            return bestPlayer;
        }
        if (bestHand == 4) {
            
        }
        return null;
    }

    public static int detectBestPokerHand(PlayerHand playerHand, CommunityCards communityCards) {
        /* 
         * Returns an integer that signifies the strongest hand the player has.
         * 1 is Royal Flush
         * 2 is Straight Flush
         * 3 is Quads
         * 4 is Full House
         * 5 is Flush
         * 6 is Straight
         * 7 is Triples
         * 8 is Two Pair
         * 9 is Pair
         * 10 is High Card
         */
        ArrayList<Card> totalCards = new ArrayList<Card>();
        totalCards.add(playerHand.getCard1());
        totalCards.add(playerHand.getCard2());
        for (int i = 0; i < communityCards.getCommunityCards().size(); i++){
            totalCards.add(communityCards.getCommunityCards().get(i));
        }
        sortCardsByVal(totalCards);

        if (checkRoyalFlush(totalCards)){
            return 1;
        }
        else if (checkStraightFlush(totalCards) != -1) {
            return 2;
        }
        else if (checkQuads(totalCards) != -1){
            return 3;
        }
        else if (checkFullHouse(totalCards) != null){
            return 4;
        }
        else if (checkFlush(totalCards) != null){
            return 5;
        }
        else if (checkStraight(totalCards) != -1) {
            return 6;
        }
        else if (checkTriples(totalCards) != -1) {
            return 7;
        }
        else if (checkTwoPair(totalCards) != null){
            return 8;
        }
        else if (checkPair(totalCards) != -1) {
            return 9;
        }
        return 10;
    }

    public static boolean checkRoyalFlush(ArrayList<Card> totalCards){
        /*
         * Total Cards is a sorted arraylist of cards in both community and the current player's hand.
         */

        ArrayList<Integer> spades = returnOnlySuits(totalCards, "S");
        ArrayList<Integer> clubs = returnOnlySuits(totalCards, "C");
        ArrayList<Integer> diamonds = returnOnlySuits(totalCards, "D");
        ArrayList<Integer> hearts = returnOnlySuits(totalCards, "H");

        ArrayList<Integer> correct = new ArrayList<Integer>();
        correct.add(1);
        for (int i = 10; i < 14; i++){
            correct.add(i);
        }

        if (spades.containsAll(correct) || clubs.containsAll(correct) || diamonds.containsAll(correct) || hearts.containsAll(correct)) {
            return true;
        }
        return false;
    }

    public static int checkStraightFlush(ArrayList<Card> totalCards){
        /* Returns the largest number of the largest straight flush they have.
         * If no straight flush, returns -1.
         */
        ArrayList<Integer> spades = returnOnlySuits(totalCards, "S");
        ArrayList<Integer> clubs = returnOnlySuits(totalCards, "C");
        ArrayList<Integer> diamonds = returnOnlySuits(totalCards, "D");
        ArrayList<Integer> hearts = returnOnlySuits(totalCards, "H");

        int iterableCounter = 0;
        if (spades.size() >= 5){
            for (int i = spades.size() - 1; i > 0; i--){
                if (spades.get(i) == spades.get(i - 1) + 1){
                    iterableCounter++;
                }
                else {
                    iterableCounter = 0;
                }
                if (iterableCounter == 4){
                    return spades.get(i + 3);
                }
            }
        }
        else if (clubs.size() >= 5) {
            for (int i = clubs.size() - 1; i > 0; i--){
                if (clubs.get(i) == clubs.get(i - 1) + 1){
                    iterableCounter++;
                }
                else {
                    iterableCounter = 0;
                }
                if (iterableCounter == 4){
                    return clubs.get(i + 3);
                }
            }
        }
        else if (diamonds.size() >= 5){
            for (int i = diamonds.size() - 1; i > 0; i--){
                if (diamonds.get(i) == diamonds.get(i - 1) + 1){
                    iterableCounter++;
                }
                else {
                    iterableCounter = 0;
                }
                if (iterableCounter == 4){
                    return diamonds.get(i + 3);
                }
            }
        }
        else if (hearts.size() >= 5){
            for (int i = hearts.size() - 1; i > 0; i--){
                if (hearts.get(i)== hearts.get(i - 1) + 1){
                    iterableCounter++;
                }
                else {
                    iterableCounter = 0;
                }
                if (iterableCounter == 4){
                    return hearts.get(i + 3);
                }
            }
        }
        return -1;
    }

    public static int checkQuads(ArrayList<Card> totalCards){
        /* If Quad exist return quad number otherwise return -1 */
        int iterableCounter = 0;
        ArrayList<Integer> ints = returnOnlyVals(totalCards);
        for (int i = 0; i < ints.size() - 1; i++){
            if (ints.get(i) == ints.get(i + 1)){
                iterableCounter++;
            }
            else {
                iterableCounter = 0;
            }
            if (iterableCounter == 3){
                return ints.get(i + 1);
            }
        }
        return -1;
    }

    public static int[] checkFullHouse(ArrayList<Card> totalCards){
         /* If Full house exists, return double number and triple number otherwise return null */
         int iterableCounter = 0;
         int triple = -1;
         int pair = -1;
         ArrayList<Integer> ints = returnOnlyVals(totalCards);
         for (int i = 0; i < ints.size() - 1; i++){
             if (ints.get(i) == ints.get(i + 1)){
                 iterableCounter++;
             }
             else {
                 iterableCounter = 0;
             }
             if (iterableCounter == 1 && pair == -1){
                pair = ints.get(i + 1);
             }
             else if (iterableCounter == 2){
                triple = ints.get(i + 1);
                if (triple == pair){
                    pair = -1;
                }
             }
         }
         int[] results = new int[2];
         if (triple != -1 && pair != -1){
            results[0] = pair;
            results[1] = triple;
            return results;
         }
         else {
            return null;
         }

    }

    public static ArrayList<Integer> checkFlush(ArrayList<Card> totalCards){
        /*
         * If flush, returns the flush cards. Otherwise returns a null.
         */
        ArrayList<Integer> spades = returnOnlySuits(totalCards, "S");
        ArrayList<Integer> clubs = returnOnlySuits(totalCards, "C");
        ArrayList<Integer> diamonds = returnOnlySuits(totalCards, "D");
        ArrayList<Integer> hearts = returnOnlySuits(totalCards, "H");

        ArrayList<Integer> fiveLargest = new ArrayList<Integer>();

        if (spades.size() >= 5){
            for (int i = spades.size() - 1; i > spades.size() - 6; i--){
                fiveLargest.add(spades.get(i));
            }
        }
        else if (clubs.size() >= 5){
            for (int i = clubs.size() - 1; i > clubs.size() - 6; i--){
                fiveLargest.add(clubs.get(i));
            }
        }
        else if (diamonds.size() >= 5) {
            for (int i = diamonds.size() - 1; i > diamonds.size() - 6; i--){
                fiveLargest.add(diamonds.get(i));
            }
        }
        else if (hearts.size() >= 5) {
            for (int i = hearts.size() - 1; i > hearts.size() - 6; i--){
                fiveLargest.add(hearts.get(i));
            }
        }
        if (fiveLargest.size() == 5){
            return fiveLargest;
        }
        return null;
    }

    public static int checkStraight(ArrayList<Card> totalCards){
        ArrayList<Integer> vals = returnOnlyVals(totalCards);
        int iterableCounter = 0;
        for (int i = vals.size() - 1; i > 0; i--){
            if (vals.get(i)== vals.get(i - 1) + 1){
                iterableCounter++;
            }
            else {
                iterableCounter = 0;
            }
            if (iterableCounter == 4){
                return vals.get(i + 3);
            }
        }
        return -1;
    }

    public static int checkTriples(ArrayList<Card> totalCards){
        ArrayList<Integer> ints = returnOnlyVals(totalCards);
        int iterableCounter = 0;
        int triple = -1;
        for (int i = 0; i < ints.size() - 1; i++){
            if (ints.get(i) == ints.get(i + 1)){
                iterableCounter++;
            }
            else {
                iterableCounter = 0;
            }
            if (iterableCounter == 2){
               triple = ints.get(i + 1);
            }
        }
        return triple;
    }

    public static ArrayList<Integer> checkTwoPair(ArrayList<Card> totalCards){
        ArrayList<Integer> ints = returnOnlyVals(totalCards);
        ArrayList<Integer> pairs = new ArrayList<Integer>();
        for (int i = 0; i < ints.size() - 1; i++){
            if (ints.get(i) == ints.get(i + 1)){
                pairs.add(ints.get(i + 1));
            }
        }
        while (pairs.size() > 2) {
            pairs.remove(0);
        }
        if (pairs.size() == 2){
            return pairs;
        }
        else {
            return null;
        }
    }

    public static int checkPair(ArrayList<Card> totalCards){
        ArrayList<Integer> ints = returnOnlyVals(totalCards);
        for (int i = ints.size() - 1; i > 0; i--){
            if (ints.get(i) == ints.get(i - 1)){
                return ints.get(i - 1);
            }
        }
        return -1;
    }

    public static int checkHigh(ArrayList<Card> totalCards){
        int[] heirarchy = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 1};
        ArrayList<Integer> ints = returnOnlyVals(totalCards);
        int highCard = 2;
        
        for (int i : ints){
            if (i == 1){
                return 1;
            }
            else {
                if (indexOf(heirarchy, i) > highCard){
                    highCard = i;
                }
            }
        }
        return highCard;
    }

    public static void sortCardsByVal(ArrayList<Card> totalCards){
        for(int i = 1; i < totalCards.size(); i++) {
            int j = i;
            while(j > 0 && totalCards.get(j).getVal() < totalCards.get(j-1).getVal()) {
                Card temp = totalCards.get(j);
                totalCards.set(j, totalCards.get(j - 1));
                totalCards.set(j - 1, temp);
                j--;
            }
        }
    }

    public static ArrayList<Integer> returnOnlySuits(ArrayList<Card> cards, String suit){
        ArrayList<Integer> suiteds = new ArrayList<Integer>();
        for (Card i : cards){
            if (i.getSuit().equals(suit)){
                suiteds.add(i.getVal());
            }
        }
        return suiteds;
    }

    public static ArrayList<Integer> returnOnlyVals(ArrayList<Card> cards) {
        ArrayList<Integer> ints = new ArrayList<Integer>();
        for (Card i : cards){
            ints.add(i.getVal());
        }
        return ints;
    }

    public static int indexOf(int[] array, int val){
        for (int i = 0; i < array.length; i++){
            if (array[i] == val){
                return i;
            }
        }
        return -1;
    }

    public static void main(String[] args){
        Deck deck = new Deck(true);
        deck.shuffleDeck();
        // PlayerHand playerOne = new PlayerHand(deck, "Bot 1");
        PlayerHand playerOne = new PlayerHand(new Card(1, "C"), new Card(1, "C"));
        // PlayerHand playerTwo = new PlayerHand(deck, "Bot 2");
        PlayerHand playerTwo = new PlayerHand(new Card(2, "C"), new Card(2, "C"));
        // PlayerHand playerThree = new PlayerHand(deck, "Bot 3");
        // PlayerHand playerFour = new PlayerHand(deck, "Bot 4");
        ArrayList<PlayerHand> allHands = new ArrayList<PlayerHand>();
        allHands.add(playerOne);
        allHands.add(playerTwo);
        // allHands.add(playerThree);
        // allHands.add(playerFour);

        CommunityCards communityCards = new CommunityCards();
        for (int i = 1; i <= 2; i++){
            communityCards.addCard(new Card(i, "C"));
            communityCards.addCard(new Card(i, "C"));
        }

        for (PlayerHand playerHand : allHands) {
            System.out.println(playerHand);
        }
        System.out.println(communityCards.getCommunityCards());
        
        System.out.println(compareHands(allHands, communityCards));
    }
}