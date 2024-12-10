# rt-at32-pulse-encoder

1. rt-thread开启脉冲编码器功能
2. at32_pulse_encoder_init
```

// 部分代码来源at32代码生成工具
rt_err_t at32_pulse_encoder_init(struct rt_pulse_encoder_device *pulse_encoder)
{
    tmr_type *tmr_x = RT_NULL;
    RT_ASSERT(pulse_encoder != RT_NULL);

    struct at32_pulse_encoder_device *at32_encoder_device;
    at32_encoder_device = (struct at32_pulse_encoder_device*)pulse_encoder;

    tmr_x = (tmr_type *)pulse_encoder->parent.user_data;
    // 初始化定时器编码器channel gpio
    at32_msp_pulse_encode_init(tmr_x);
    //  分频为2
    //  增量编码器 对上下边沿都敏感 ，每组脉冲会导致计数2次
    tmr_base_init(tmr_x, AUTO_RELOAD_VALUE, TMR_CLOCK_DIV2);
    tmr_cnt_dir_set(tmr_x, TMR_COUNT_UP);
  
    tmr_clock_source_div_set(TMR3, TMR_CLOCK_DIV1);
    tmr_period_buffer_enable(TMR3, FALSE);

    tmr_sub_sync_mode_set(TMR3, FALSE);
    tmr_primary_mode_select(TMR3, TMR_PRIMARY_SEL_RESET);

    tmr_input_config_type  tmr_input_struct;
    /* configure encoder mode */
    tmr_input_struct.input_channel_select = TMR_SELECT_CHANNEL_1;
    tmr_input_struct.input_mapped_select = TMR_CC_CHANNEL_MAPPED_DIRECT;
    tmr_input_struct.input_polarity_select = TMR_INPUT_RISING_EDGE;
    tmr_input_struct.input_filter_value = 0;
    tmr_input_channel_init(tmr_x, &tmr_input_struct, TMR_CHANNEL_INPUT_DIV_1);

    tmr_input_struct.input_channel_select = TMR_SELECT_CHANNEL_2;
    tmr_input_struct.input_mapped_select = TMR_CC_CHANNEL_MAPPED_DIRECT;
    tmr_input_struct.input_polarity_select = TMR_INPUT_RISING_EDGE;
    tmr_input_struct.input_filter_value = 0;
    tmr_input_channel_init(tmr_x, &tmr_input_struct, TMR_CHANNEL_INPUT_DIV_1);
    
    // TMR_ENCODER_MODE_A | TMR_ENCODER_MODE_B 只使用一个信号通道, 如果不分频的情况下，每次旋转默认+2
    // TMR_ENCODER_MODE_C  监控2个channel信号通道, 如果不分频的情况下，每次旋转默认+4
    // 顺时针和逆时针问题
    // 调整2个channel的捕获， 其中某一个为 TMR_INPUT_FALLING_EDGE 下降沿视为有效电平
    tmr_encoder_mode_config(tmr_x, TMR_ENCODER_MODE_A, TMR_INPUT_FALLING_EDGE, TMR_INPUT_RISING_EDGE);

    LOG_D("%s init success", at32_encoder_device->name);
    return RT_EOK;
}
```
