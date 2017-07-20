;;
(add-to-list'load-path "~/.emacs.d/twittering-mode")

(require 'twittering-mode)
(setq twittering-use-master-password t)
(setq twittering-use-ssl t)

(setq twittering-username "username")
(setq twittering-password "password")
;(setq twittering-auth-method 'xauth)
; (setq twittering-cert-file "/etc/ssl/certs/ca-certificates.crt")
(setq twittering-timer-interval 300)
(setq twittering-number-of-tweets-on-retrieval 50)
(setq twittering-initial-timeline-spec-string
  '(":mentions"
    ":direct_messages"
    ":home"))
(setq twittering-icon-mode nil)
;;(setq twittering-use-icon-storage t)
(setq twittering-proxy-use nil)

