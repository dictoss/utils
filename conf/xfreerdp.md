
# freerdpの起動コマンドライン

## 自分が使っているコマンドライン

- `$ xfreerdp /size:1350x700 /bpp:16 /kbd-type:7 /kbd-subtype:2 /kbd-fn-key:12 /sound:sys:alsa /network:2 +clipboard +fonts -wallpaper -menu-anims /gdi:hw /cert-ignore /cert-tofu /sec:tls /u:${RDP_USER} /v:${TARGET_HOST}:3389`