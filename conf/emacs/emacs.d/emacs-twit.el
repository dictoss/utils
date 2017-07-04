;;
(require 'twittering-mode)
(setq twittering-username "username")
(setq twittering-password "password")
(setq twittering-auth-method 'xauth)
(setq twittering-timer-interval 300)
(setq twittering-number-of-tweets-on-retrieval 50)
(setq twittering-initial-timeline-spec-string
  '(":mentions"
    ":direct_messages"
    ":home"))
